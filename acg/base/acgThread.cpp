#include "../stdafx.h"
#include "acgThread.h"
#include "acgCurThread.h"



namespace acg
{
	namespace base
	{

		namespace CurThread
		{
			ACG_TLS DWORD gt_dwThreadId=0;
			ACG_TLS TCHAR gt_szThreadId[32] = {};
			ACG_TLS DWORD gt_nThreadIdLength=6;
			ACG_TLS LPCTSTR pszThreadName=TEXT("unknown");

			void CacheThreadId()
			{
				if (0 == gt_dwThreadId)
				{
					gt_dwThreadId = GetCurrentThreadId();
					ACG_CHECK_STR(StringCchPrintf(gt_szThreadId, ACG_COUNTOF(gt_szThreadId), _T("%5d"), gt_dwThreadId));
					ACG_CHECK_STR(StringCchLength(gt_szThreadId, ACG_COUNTOF(gt_szThreadId), (size_t*)(&gt_nThreadIdLength)));
				}
			}

		}






	}
}
