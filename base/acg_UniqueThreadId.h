#pragma once

#include "acg_base_lib.h"
#include "acg_dll.h"



namespace acg
{
	namespace base
	{
		namespace CurThread
		{
			ACG_DLL  LONG GetCurUniqueThreadId();
            const    LONG g_nMainThreadId = 1;
		}

	}
}


