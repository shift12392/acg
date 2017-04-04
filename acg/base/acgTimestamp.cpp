

#include "../stdafx.h"
#include "acgTimestamp.h"

#include <inttypes.h>


namespace acg
{
	namespace base
	{
		string CACGTimestamp::ToString() const 
		{
			char buf[32] = { 0 };
			LONGLONG seconds = m_nMicroSecondsSinceEpoch / m_nMicroSecondsPerSecond;
			LONGLONG microseconds = m_nMicroSecondsSinceEpoch % m_nMicroSecondsPerSecond;
			StringCchPrintfA(buf, sizeof(buf) - 1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
			return buf;
		}

		string CACGTimestamp::ToFormattedString(BOOL bShowMicroseconds ) const
		{
			char buf[32] = { 0 };
			FILETIME ft = {};
			SYSTEMTIME st = {};
			UnixToFileTime(m_nMicroSecondsSinceEpoch, &ft);
			FileTimeToSystemTime(&ft, &st);

			if (bShowMicroseconds)
			{
				LONGLONG microseconds= m_nMicroSecondsSinceEpoch % m_nMicroSecondsPerSecond;
				StringCchPrintfA(buf, sizeof(buf)-1, "%4d%02d%02d %02d:%02d:%02d.%06d",
					st.wYear , st.wMonth, st.wDay,
					st.wHour, st.wMinute, st.wSecond,
					microseconds);
			}
			else
			{
				StringCchPrintfA(buf, sizeof(buf) - 1, "%4d%02d%02d %02d:%02d:%02d.%06d",
					st.wYear, st.wMonth, st.wDay,
					st.wHour, st.wMinute, st.wSecond);
			}
			return buf;
		}
	}
}