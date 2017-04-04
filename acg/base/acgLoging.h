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
				inline SourceFile(const char(&arr)[N])
					:data_(arr)
					, size_(N)
				{
					const char* slash = strrchr(data_, '/'); // builtin function
					if (slash)
					{
						data_ = slash + 1;
						size_ -= static_cast<int>(data_ - arr);
					}
				}


				explicit SourceFile(const char* filename)
					: data_(filename)
				{
					const char* slash = strrchr(filename, '/');
					if (slash)
					{
						data_ = slash + 1;
					}
					size_ = static_cast<int>(strlen(data_));
				}


				const char* data_;
				int size_;

			};

			Logger(SourceFile file, int line);
			Logger(SourceFile file, int line, LogLevel level);
			Logger(SourceFile file, int line, LogLevel level, const char* func);
			Logger(SourceFile file, int line, bool toAbort);
			~Logger();


			LogStream& stream() { return impl_.stream_; }

			static LogLevel logLevel();
			static void     SetLogLevel(LogLevel level);


			typedef void(*OutputFunc)(const char *msg, int len);
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


		extern Logger::LogLevel g_logLevel;

		Logger::LogLevel Logger::logLevel()
		{
			return g_logLevel;
		}


		//得到nLastError的错误描述字符串，并把错误描述字符串保存到线程局部缓存区中
		const char* strerror_tl(int nLastError);

	}
}
