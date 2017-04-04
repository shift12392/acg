

#include "../stdafx.h"
#include "acgLoging.h"
#include "acgException.h"



namespace acg
{
	namespace base
	{

		ACG_TLS char gt_szLastError[1024];
		ACG_TLS char gt_szTime[32];
		ACG_TLS int  gt_nLastSecond;

		const char* strerror_tl(int nLastError)
		{
			USES_CONVERSION;
			CACGException e(nLastError);
			LPCTSTR lpEr = e.GetReason();
			if (NULL != lpEr)
			{
				ACG_CHECK_STR(StringCchCopyA(gt_szLastError, g_ErrBuf_MaxLen, T2A(lpEr)))
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
		LLogger::LogLevel g_logLevel = Logger::em_INFO;
#endif // DEBUG

		//LOG等级字符串
		const char* LogLevelName[Logger::em_NUM_LOG_LEVELS] =            //这个设计要求enum中值必须连续
		{
			"TRACE ",
			"DEBUG ",
			"INFO  ",
			"WARN  ",
			"ERROR ",
			"FATAL ",
		};


		// helper class for known string length at compile time
		class T
		{
		public:
			T(const char* str, unsigned len)
				:str_(str),
				len_(len)
			{
				ACG_ASSERT(strlen(str) == len_);
			}

			const char* str_;
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

		void defaultOutput(const char* msg, int len)
		{
			ACG_ASSERT(FALSE);
		}
		void defaultFlush()
		{
			ACG_ASSERT(FALSE);
		}
		Logger::OutputFunc g_output = defaultOutput;
		Logger::FlushFunc g_flush = defaultFlush;


		Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
			: time_(CACGTimestamp::now()),
			stream_(),
			level_(level),
			line_(line),
			basename_(file)
		{

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

				StringCchPrintfA(gt_szTime, sizeof(gt_szTime) - 1, "%4d%02d%02d %02d:%02d:%02d.%02d",
					st.wYear, st.wMonth, st.wDay,
					st.wHour, st.wMinute, st.wSecond);
			}

		}




		void Logger::SetLogLevel(Logger::LogLevel level)
		{
			g_logLevel = level;
	    }

	}
}