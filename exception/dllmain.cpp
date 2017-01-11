

#include "../acg/stdafx.h"
#define ACG_EXPORT                      //在这定义ACG_EXPORT
#include "../acg/base/acgException.h"
#include "../acg/base/acg_DbgOut.h"
#include <assert.h>





BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	{

		assert(false);

		//为主线程安装结构化异常的C++拦截器
		_set_se_translator(acg::base::ACG_SEH_Handle);
#ifdef DEBUG
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
		_set_se_translator(acg::base::ACG_SEH_Handle);
#ifdef DEBUG
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
	}
	break;
	case DLL_PROCESS_DETACH:
	{
	}
	break;
	}
	return TRUE;
}
