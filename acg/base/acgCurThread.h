#pragma once
#include "acg_UniqueThreadId.h"

namespace acg
{
	namespace base
	{
		namespace CurThread
		{
#ifdef DEBUG
			ACG_INLINE BOOL IsMainThread()
			{
				if (0 == GetCurUniqueThreadId())
					return TRUE;
				else
					return FALSE;
			}
#endif // DEBUG
			extern ACG_TLS DWORD gt_dwThreadId;
			extern ACG_TLS TCHAR gt_szThreadId[32];
			extern ACG_TLS DWORD gt_nThreadIdLength;
			extern ACG_TLS LPCTSTR pszThreadName;
			void CacheThreadId();

			ACG_INLINE DWORD GetTid()
			{
				if (0 == gt_dwThreadId)
					CacheThreadId();
				return gt_dwThreadId;
			}

			ACG_INLINE LPCTSTR GetTidString()
			{
				return gt_szThreadId;
			}

			ACG_INLINE DWORD GetTidStringLength()
			{
				return gt_nThreadIdLength;
			}

			ACG_INLINE LPCTSTR GetName()
			{
				return pszThreadName;
			}
		}
	}
}