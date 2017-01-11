#pragma once

#include "../stdafx.h"
#include "acg_dll.h"



namespace acg
{
	namespace base
	{
		namespace CurThread
		{
#ifdef DEBUG
			ACG_DLL  LONG GetCurUniqueThreadId();
#endif // DEBUG
		}

	}
}


