
//文件描述：调试输出支持模块封装

#pragma once

#include "acg_base_lib.h"
#define ACG_DBGOUT_BUF_LEN 2048


#ifdef _DEBUG
#define ACG_ASSERT(s) if(!(s)) {::DebugBreak();}
#define ACG_CHECK_STR(ret) if (S_OK != (ret)) ACG_ASSERT(FALSE);          //检查安全字符串函数返回值，如果失败，则杀死进程
#else
#define ACG_ASSERT(s) 
#define ACG_CHECK_STR(ret) if (S_OK != (ret)) exit(1);
#endif

namespace acg
{
	namespace base
	{
		namespace
		{
			ACG_INLINE void ACG_CDECL ACGDbgOutW(LPCWSTR pszFormat, ...)
			{//UNICODE版的待时间的Trace
				va_list va;
				va_start(va, pszFormat);

				WCHAR pBuffer[ACG_DBGOUT_BUF_LEN] = {};

				if (S_OK != ::StringCchVPrintfW(pBuffer, ACG_COUNTOF(pBuffer), pszFormat, va))
				{
					va_end(va);
					return;
				}
				va_end(va);
				OutputDebugStringW(pBuffer);
			}
		}
	}
}



//普通的调试输出支持
#if defined(DEBUG) || defined(_DEBUG)
#define ACG_DBGOUTW(...)     acg::base::ACGDbgOutW(__VA_ARGS__)
#define ACG_DBGOUT_LINEW()	 ACG_DBGOUTW(L"%s(%d):",__WFILE__, __LINE__)
#else
#define ACG_DBGOUT(...)
#define ACG_DBGOUTA(...)
#define ACG_DBGOUTW(...)
#define ACG_DBGOUT_LINEW()
#endif


//输出异常CACGException
#ifdef _DEBUG
#define ACG_DBGOUT_EXPW(Fun,e) acg::base::ACGDbgOutW(L"%s(%d):%s函数捕获异常(0x%08X):%s\n",__WFILE__,__LINE__,L#Fun,(e).GetErrCode(),(e).GetReason())
#else
#define ACG_DBGOUT_EXPW(Fun,e)
#endif
