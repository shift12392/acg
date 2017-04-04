#pragma once
#include "acg_UniqueThreadId.h"

namespace acg
{
	namespace base
	{
		namespace CurThread
		{
			ACG_INLINE BOOL IsMainThread()
			{
				if (0 == GetCurUniqueThreadId())
					return TRUE;
				else
					return FALSE;
			}
			extern ACG_TLS LONG gt_dwThreadId;
			extern ACG_TLS char  gt_szThreadId[64];
			extern ACG_TLS size_t gt_nThreadIdLength;
			extern ACG_TLS LPCSTR pszThreadName;
			void CacheThreadId();

			ACG_INLINE LONG GetTid()
			{
				if (0 == gt_dwThreadId)
					CacheThreadId();
				return gt_dwThreadId;
			}

			ACG_INLINE LPSTR GetTidString()
			{
				return gt_szThreadId;
			}

			ACG_INLINE size_t GetTidStringLength()
			{
				return gt_nThreadIdLength;
			}

			ACG_INLINE LPCSTR GetName()
			{
				return pszThreadName;
			}
		}
	}
}