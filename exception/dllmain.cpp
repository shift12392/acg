

#include "../acg/stdafx.h"
#define ACG_EXPORT                      //���ⶨ��ACG_EXPORT
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

		//Ϊ���̰߳�װ�ṹ���쳣��C++������
		_set_se_translator(acg::base::ACG_SEH_Handle);
#ifdef DEBUG
		TCHAR pModuleName[MAX_PATH] = {};
		GetModuleFileName(hModule, pModuleName, MAX_PATH);
		ACG_DBGOUTW(_T("ģ��(%s)��Ϊ����[%u]�����߳�[0x%x]����C++��ʽ���ؽṹ���쳣����,\
�߳̿���ʹ��CGRSException��������\n")
, pModuleName, GetCurrentProcessId(), GetCurrentThreadId());
#endif
	}
	break;
	case DLL_THREAD_ATTACH:
	{
		//Ϊ�����̰߳�װ�ṹ���쳣��C++������
		_set_se_translator(acg::base::ACG_SEH_Handle);
#ifdef DEBUG
		TCHAR pModuleName[MAX_PATH] = {};
		GetModuleFileName(hModule, pModuleName, MAX_PATH);
		ACG_DBGOUTW(_T("ģ��(%s)��Ϊ����[%u]���߳�[0x%x]����C++��ʽ���ؽṹ���쳣����,\
�߳̿���ʹ��CGRSException��������\n")
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
