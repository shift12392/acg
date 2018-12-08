

#include "acg_base_lib.h"
#include "acgLoging.h"
#include "acgException.h"
#include "acgCurThread.h"
#include "acg_DbgOut.h"


#ifdef DEBUG
#include <iostream>
#endif // DEBUG



namespace acg
{
	namespace base
	{

		ACG_TLS WCHAR gt_szLastError[1024];
		ACG_TLS WCHAR gt_szTime[32];
		ACG_TLS int  gt_nLastSecond;

		const WCHAR* strerror_tl(int nLastError)
		{
			USES_CONVERSION;
			CACGException e(nLastError);
			LPCWSTR lpEr = e.GetReason();
			if (NULL != lpEr)
			{
				ACG_CHECK_STR(StringCchCopyW(gt_szLastError, g_ErrBuf_MaxLen, lpEr))
				return gt_szLastError;
			}
			else
			{
				ACG_ASSERT(FALSE);
				return NULL;
			}
		}

#ifdef DEBUG
		Logger::LogLevel g_logLevel = Logger::em_DEBUG;
#else
		Logger::LogLevel g_logLevel = Logger::em_INFO;
#endif // DEBUG

		//LOG等级字符串
		const WCHAR* LogLevelName[Logger::em_NUM_LOG_LEVELS] =            //这个设计要求enum中值必须连续
		{
			L"TRACE ",
			L"DEBUG ",
			L"INFO  ",
			L"WARN  ",
			L"ERROR ",
			L"FATAL ",
		};


		// helper class for known string length at compile time
		class T
		{
		public:
			T(const WCHAR* str, unsigned len)
				:str_(str),
				len_(len)
			{
				ACG_ASSERT(wcslen(str) == len_);
			}

			const WCHAR* str_;
			const unsigned len_;
		};

		inline LogStream& operator<<(LogStream& s, T v)
		{
			s.append(v.str_, v.len_);
			return s;
		}


		inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
		{
			s.append(v.data_, v.size_);
			return s;
		}




		void defaultOutput(const WCHAR* msg, int len)
		{
			//ACG_ASSERT(FALSE);
            std::wcout << msg;

            if (Logger::em_DEBUG == g_logLevel)
                ACG_DBGOUTW(msg);

		}
		void defaultFlush()
		{
			ACG_ASSERT(FALSE);       
		}

		Logger::OutputFunc g_outputFunc = defaultOutput;
		Logger::FlushFunc g_flushFunc = defaultFlush;


        void Logger::setOutput(OutputFunc funOutput)
        {
			g_outputFunc = funOutput;
        }
        void Logger::setFlush(FlushFunc funFlush)
        {
			g_flushFunc = funFlush;
        }

		Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
			: time_(CACGTimestamp::now()),
			stream_(),
			level_(level),
			line_(line),
			basename_(file)
		{
            formatTime();
            CurThread::GetTid();
            stream_ << T(CurThread::GetTidString(), static_cast<unsigned>(CurThread::GetTidStringLength()));
            stream_ << T(LogLevelName[level], 6);
            if (savedErrno != 0)
            {
                stream_ << strerror_tl(savedErrno) << L" (errno=" << savedErrno << L") ";
            }
		}


		void Logger::Impl::formatTime()
		{
			LONGLONG  microSecondsSinceEpoch = time_.GetSysTimeMicros();
			int       seconds = static_cast<int>(microSecondsSinceEpoch / CACGTimestamp::m_nMicroSecondsPerSecond);
			int       microSecond= static_cast<int>(microSecondsSinceEpoch % CACGTimestamp::m_nMicroSecondsPerSecond);

			//如果秒数不变，则只需格式化微妙
			if (seconds > gt_nLastSecond)        
			{
				FILETIME ft = {};
				SYSTEMTIME st = {};
				CACGTimestamp::UnixToFileTime(microSecondsSinceEpoch, &ft);
				FileTimeToSystemTime(&ft, &st);

				StringCchPrintfW(gt_szTime, ACG_COUNTOF(gt_szTime) - 1, L"%4d%02d%02d %02d:%02d:%02d",
					st.wYear, st.wMonth, st.wDay,
					st.wHour, st.wMinute, st.wSecond);
			}

            //格式化微妙

            Fmt us(L".%06dZ ", microSecond);
            ACG_ASSERT(us.length() == 9);
            stream_ << T(gt_szTime, 17) << T(us.data(), 9);
		}


        void Logger::Impl::finish()
        {
            stream_ << L" - " << basename_ << L':' << line_ << L'\n';
        }


        Logger::Logger(SourceFile file, DWORD line)
            : impl_(em_INFO, 0, file, line)
        {
        }

        Logger::Logger(SourceFile file, DWORD line, LogLevel level, const WCHAR* func)
            : impl_(level, 0, file, line)
        {
            impl_.stream_ << func << L' ';
        }

        Logger::Logger(SourceFile file, DWORD line, LogLevel level)
            : impl_(level, 0, file, line)
        {
        }

        Logger::Logger(SourceFile file, DWORD line, bool toAbort)
            : impl_(toAbort ? em_FATAL : em_ERROR, GetLastError(), file, line)
        {
        }

        Logger::~Logger()
        {
            impl_.finish();
            const LogStream::Buffer& buf(stream().buffer());
			g_outputFunc(buf.data(), buf.length());
            if (impl_.level_ == em_FATAL)
            {
				g_flushFunc();
                abort();
            }
			else if (impl_.level_ == em_ERROR)
			{
				g_flushFunc();
				ACG_ASSERT(FALSE);
			}
        }



		void Logger::SetLogLevel(Logger::LogLevel level)
		{
			g_logLevel = level;
	    }

	}
}