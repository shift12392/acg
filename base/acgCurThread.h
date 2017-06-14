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
				if (g_nMainThreadId == GetCurUniqueThreadId())
					return TRUE;
				else
					return FALSE;
			}
			extern ACG_TLS LONG gt_nThreadId;
			extern ACG_TLS WCHAR  gt_szThreadId[64];
			extern ACG_TLS size_t gt_nThreadIdLength;
			extern ACG_TLS LPCWSTR pszThreadName;
			void CacheThreadId();

			ACG_INLINE LONG GetTid()
			{
				if (0 == gt_nThreadId)
					CacheThreadId();
				return gt_nThreadId;
			}

			ACG_INLINE LPWSTR GetTidString()
			{
				return gt_szThreadId;
			}

			ACG_INLINE size_t GetTidStringLength()
			{
				return gt_nThreadIdLength;
			}

			ACG_INLINE LPCWSTR GetName()
			{
				return pszThreadName;
			}
		}
	}
}