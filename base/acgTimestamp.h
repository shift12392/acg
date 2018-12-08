#pragma once


#include "acgCopyable.h"
#include "acg_type.h"
#include "acg_DbgOut.h"

#include <algorithm>

namespace acg
{
	namespace base
	{
		//namespace time
		//{
		//	// 获取Unix时间戳，单位微秒(us)，线程安全
		//	inline LONGLONG GetSysTimeMicros()
		//	{
		//		
		//		FILETIME ft = {};
		//		LARGE_INTEGER li = {};
		//		LONGLONG tt = 0;
		//		GetSystemTimeAsFileTime(&ft);
		//		li.LowPart = ft.dwLowDateTime;
		//		li.HighPart = ft.dwHighDateTime;
		//		// 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
		//		// 116444736000000000UL: 从1601年1月1日0:0:0:000到1970年1月1日0:0:0:000的时间(单位100ns)
		//		tt = (li.QuadPart - 116444736000000000UL) / 10;   
		//		return tt;
		//	}
		//}


		class CACGTimestamp :public CACGCopyable
		{
		public:
			CACGTimestamp()
				:m_nMicroSecondsSinceEpoch(0)
			{ }
			CACGTimestamp(LONGLONG microSecondsSinceEpochArg)
				:m_nMicroSecondsSinceEpoch(microSecondsSinceEpochArg)
			{ }

			void swap(CACGTimestamp &that)
			{
				std::swap(m_nMicroSecondsSinceEpoch, that.m_nMicroSecondsSinceEpoch);
			}

			BOOL IsVaild() const { return m_nMicroSecondsSinceEpoch > 0 ? FALSE : TRUE; }

			LONGLONG GetTimestamp() const { return m_nMicroSecondsSinceEpoch; }

			CACGString ToString() const;
            CACGString ToFormattedString(BOOL bShowMicroseconds = TRUE) const;

			//得到当前时间戳
			static CACGTimestamp now()
			{
				return CACGTimestamp(GetSysTimeMicros());
			}

			//得到单位微妙时间戳
			static inline LONGLONG GetSysTimeMicros()
			{
					FILETIME ft = {};
					GetSystemTimeAsFileTime(&ft);
					return FileTimeToUnix(ft);
			}

			//FILETIME时间格式转换为unix时间戳
			static inline LONGLONG FileTimeToUnix(const FILETIME &time)
			{
				LARGE_INTEGER li = {};
				LONGLONG tt = 0;
				li.LowPart = time.dwLowDateTime;
				li.HighPart = time.dwHighDateTime;
				// 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
				// 116444736000000000UL: 从1601年1月1日0:0:0:000到1970年1月1日0:0:0:000的时间(单位100ns)
				tt = (li.QuadPart - 116444736000000000UL) / 10;
				return tt;
			}

			static inline void UnixToFileTime(const LONGLONG time,LPFILETIME pFileTime)
			{
				ACG_ASSERT(pFileTime != NULL);
				LARGE_INTEGER li = {};
				li.QuadPart = time * 10 + 116444736000000000UL;
				(*pFileTime).dwHighDateTime = li.HighPart;
				(*pFileTime).dwLowDateTime = li.LowPart;
			}

			//秒和微妙换算
			static const int m_nMicroSecondsPerSecond = 1000 * 1000;

		private:
			LONGLONG m_nMicroSecondsSinceEpoch;                    //这是标准时间，不是本地时间
		};


		inline bool operator<(CACGTimestamp lhs, CACGTimestamp rhs)
		{
			return lhs.GetSysTimeMicros() < rhs.GetSysTimeMicros();
		}

		inline bool operator>(CACGTimestamp lhs, CACGTimestamp rhs)
		{
			return lhs.GetSysTimeMicros() > rhs.GetSysTimeMicros();
		}

		inline bool operator==(CACGTimestamp lhs, CACGTimestamp rhs)
		{
			return lhs.GetSysTimeMicros() == rhs.GetSysTimeMicros();
		}

		inline bool operator!=(CACGTimestamp lhs, CACGTimestamp rhs)
		{
			return lhs.GetSysTimeMicros() != rhs.GetSysTimeMicros();
		}

		///
		/// Gets time difference of two timestamps, result in seconds.
		///
		/// @param high, low
		/// @return (high-low) in seconds
		/// @c double has 52-bit precision, enough for one-microsecond
		/// resolution for next 100 years.
		inline double timeDifference(CACGTimestamp high, CACGTimestamp low)
		{
			int64_t diff = high.GetSysTimeMicros() - low.GetSysTimeMicros();
			return static_cast<double>(diff) / CACGTimestamp::m_nMicroSecondsPerSecond;
		}

		///
		/// Add @c seconds to given timestamp.
		///
		/// @return timestamp+seconds as Timestamp
		///
		inline CACGTimestamp addTime(CACGTimestamp timestamp, double seconds)
		{
			int64_t delta = static_cast<int64_t>(seconds * CACGTimestamp::m_nMicroSecondsPerSecond);
			return CACGTimestamp(timestamp.GetSysTimeMicros() + delta);
		}

	}
}
