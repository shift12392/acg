

#include "../base/acg_base_lib.h"
#define ACG_EXPORT                      //在这定义ACG_EXPORT
#include "../base/acgException.h"
#include "../base/acg_UniqueThreadId.h"
#include "../base/acg_DbgOut.h"


//注：dll延迟加载问题怎么解决


void ACG_SEH_Handle(unsigned int code, struct _EXCEPTION_POINTERS *ep)
{//注意下面的异常抛出方式，这样保证所有的异常在栈上并且是自动对象
	throw acg::base::CACGException(code, ep);
}


namespace
{
	DWORD g_dwTlsID = TLS_OUT_OF_INDEXES;
	LONG  g_nUniqueThreadId = acg::base::CurThread::g_nMainThreadId;           //用于标识全局唯一的线程id
}
namespace acg
{
	namespace base
	{
		namespace CurThread
		{
			LONG GetCurUniqueThreadId()
			{
				return *(LONG*)(TlsGetValue(g_dwTlsID));
			}
		}
	}
}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		//为主线程安装结构化异常的C++拦截器
		_set_se_translator(ACG_SEH_Handle);


		//给主线程设置用于标识全局唯一的线程id
		LONG *gt_pNUniqueThreadId = new LONG;
		*gt_pNUniqueThreadId = g_nUniqueThreadId;

		g_dwTlsID = TlsAlloc();
		if (g_dwTlsID != TLS_OUT_OF_INDEXES)
		{
			ACG_ASSERT(TlsSetValue(g_dwTlsID, gt_pNUniqueThreadId));
		}
		else
		{
			ACG_ASSERT(FALSE);
            exit(1);
		}

#ifdef DEBUG
		//调试输出
		TCHAR pModuleName[MAX_PATH] = {};
		GetModuleFileName(hModule, pModuleName, MAX_PATH);
		ACG_DBGOUTW(_T("模块(%s)已为进程[%u]的主线程[0x%x]设置C++方式拦截结构化异常处理,\
线程可以使用CGRSException对象引用\n")
, pModuleName, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	}
	break;
	case DLL_THREAD_ATTACH:
	{
		//为其它线程安装结构化异常的C++拦截器
		_set_se_translator(ACG_SEH_Handle);


		//给新线程设置用于标识全局唯一的线程id
		InterlockedIncrement(&g_nUniqueThreadId);     //++
		LONG *gt_pNUniqueThreadId = new LONG;
		*gt_pNUniqueThreadId = g_nUniqueThreadId;
		TlsSetValue(g_dwTlsID, gt_pNUniqueThreadId);

#ifdef DEBUG
		//调试输出
		TCHAR pModuleName[MAX_PATH] = {};
		GetModuleFileName(hModule, pModuleName, MAX_PATH);
		ACG_DBGOUTW(_T("模块(%s)已为进程[%u]的线程[0x%x]设置C++方式拦截结构化异常处理,\
线程可以使用CGRSException对象引用\n")
, pModuleName, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	}
	break;
	case DLL_THREAD_DETACH:
	{
		if (TLS_OUT_OF_INDEXES != g_dwTlsID)
			TlsFree(g_dwTlsID);
	}
	break;
	case DLL_PROCESS_DETACH:
	{
		if (TLS_OUT_OF_INDEXES != g_dwTlsID)
			TlsFree(g_dwTlsID);
	}
	break;
	}
	return TRUE;
}
