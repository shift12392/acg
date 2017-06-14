#pragma once


#include "acgLogStream.h"
#include "acgTimestamp.h"



namespace acg
{
	namespace base
	{
		class Logger
		{
		public:
			enum LogLevel
			{
				em_TRACE,
				em_DEBUG,
				em_INFO,
				em_WARN,
				em_ERROR,
				em_FATAL,
				em_NUM_LOG_LEVELS,         
			};

			class SourceFile
			{
			public:
				template <int N>
				inline SourceFile(const WCHAR(&arr)[N])
					:data_(arr)
					, size_(N)
				{
					const WCHAR* slash = wcsrchr(data_, L'\\'); // builtin function
					if (slash)
					{
						data_ = slash + 1;
						size_ -= static_cast<int>(data_ - arr);
					}
				}


				explicit SourceFile(const WCHAR* filename)
					: data_(filename)
				{
					const WCHAR* slash = wcsrchr(filename, L'\\');
					if (slash)
					{
						data_ = slash + 1;
					}
					size_ = static_cast<int>(wcslen(data_));
				}


				const WCHAR* data_;
				int size_;

			};

			Logger(SourceFile file, DWORD line);
			Logger(SourceFile file, DWORD line, LogLevel level);
			Logger(SourceFile file, DWORD line, LogLevel level, const WCHAR* func);
			Logger(SourceFile file, DWORD line, bool toAbort);
			~Logger();


			LogStream& stream() { return impl_.stream_; }

			static LogLevel logLevel();
			static void     SetLogLevel(LogLevel level);


			typedef void(*OutputFunc)(const WCHAR *msg, int len);
			typedef void(*FlushFunc)();
			static void setOutput(OutputFunc);
			static void setFlush(FlushFunc);

			class Impl
			{
			public:
				typedef Logger::LogLevel LogLevel;
				Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
				void formatTime();
				void finish();

				CACGTimestamp time_;
				LogStream     stream_;
				LogLevel      level_;
				int           line_;
				SourceFile basename_;
			};




			Impl  impl_;
		};

        //全局的日志等级
		extern Logger::LogLevel g_logLevel;

		inline Logger::LogLevel Logger::logLevel()
		{
			return g_logLevel;
		}


#define LOG_TRACE if (acg::base::Logger::logLevel() <= acg::base::Logger::em_TRACE) \
  acg::base::Logger(__FILEW__, __LINE__, acg::base::Logger::em_TRACE, __FUNCTIONW__).stream()
#define LOG_DEBUG if (acg::base::Logger::logLevel() <= acg::base::Logger::em_DEBUG) \
  acg::base::Logger(__FILEW__, __LINE__, acg::base::Logger::em_DEBUG, __FUNCTIONW__).stream()
#define LOG_INFO if (acg::base::Logger::logLevel() <= acg::base::Logger::em_INFO) \
  acg::base::Logger(__FILEW__, __LINE__).stream()
#define LOG_WARN acg::base::Logger(__FILEW__, __LINE__, acg::base::Logger::em_WARN).stream()
#define LOG_ERROR acg::base::Logger(__FILEW__, __LINE__, acg::base::Logger::em_ERROR).stream()
#define LOG_FATAL acg::base::Logger(__FILEW__, __LINE__, acg::base::Logger::em_FATAL).stream()
#define LOG_SYSERR acg::base::Logger(__FILEW__, __LINE__, false).stream()
#define LOG_SYSFATAL acg::base::Logger(__FILEW__, __LINE__, true).stream()






		//得到nLastError的错误描述字符串，并把错误描述字符串保存到线程局部缓存区中
		const WCHAR* strerror_tl(int nLastError);

	}
}
