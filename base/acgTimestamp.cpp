

#include "acg_base_lib.h"
#include "acgTimestamp.h"

#include <inttypes.h>


namespace acg
{
	namespace base
	{
		CACGString CACGTimestamp::ToString() const 
		{
			WCHAR buf[32] = { 0 };
			LONGLONG seconds = m_nMicroSecondsSinceEpoch / m_nMicroSecondsPerSecond;
			LONGLONG microseconds = m_nMicroSecondsSinceEpoch % m_nMicroSecondsPerSecond;
            StringCchPrintfW(buf, sizeof(buf) - 1, L"%" PRId64 ".%06" PRId64 "", seconds, microseconds);
			return buf;
		}

        CACGString CACGTimestamp::ToFormattedString(BOOL bShowMicroseconds ) const
		{
			WCHAR buf[32] = { 0 };
			FILETIME ft = {};
			SYSTEMTIME st = {};
			UnixToFileTime(m_nMicroSecondsSinceEpoch, &ft);
			FileTimeToSystemTime(&ft, &st);

			if (bShowMicroseconds)
			{
				LONGLONG microseconds= m_nMicroSecondsSinceEpoch % m_nMicroSecondsPerSecond;
				StringCchPrintfW(buf, sizeof(buf)-1, L"%4d%02d%02d %02d:%02d:%02d.%06d",
					st.wYear , st.wMonth, st.wDay,
					st.wHour, st.wMinute, st.wSecond,
					microseconds);
			}
			else
			{
				StringCchPrintfW(buf, sizeof(buf) - 1, L"%4d%02d%02d %02d:%02d:%02d.%06d",
					st.wYear, st.wMonth, st.wDay,
					st.wHour, st.wMinute, st.wSecond);
			}
			return buf;
		}
	}
}