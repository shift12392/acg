

#include "../base/acg_base_lib.h"
#define ACG_EXPORT                      //���ⶨ��ACG_EXPORT
#include "../base/acgException.h"
#include "../base/acg_UniqueThreadId.h"
#include "../base/acg_DbgOut.h"


//ע��dll�ӳټ���������ô���


void ACG_SEH_Handle(unsigned int code, struct _EXCEPTION_POINTERS *ep)
{//ע��������쳣�׳���ʽ��������֤���е��쳣��ջ�ϲ������Զ�����
	throw acg::base::CACGException(code, ep);
}


namespace
{
	DWORD g_dwTlsID = TLS_OUT_OF_INDEXES;
	LONG  g_nUniqueThreadId = acg::base::CurThread::g_nMainThreadId;           //���ڱ�ʶȫ��Ψһ���߳�id
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
		//Ϊ���̰߳�װ�ṹ���쳣��C++������
		_set_se_translator(ACG_SEH_Handle);


		//�����߳��������ڱ�ʶȫ��Ψһ���߳�id
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
		//�������
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
		_set_se_translator(ACG_SEH_Handle);


		//�����߳��������ڱ�ʶȫ��Ψһ���߳�id
		InterlockedIncrement(&g_nUniqueThreadId);     //++
		LONG *gt_pNUniqueThreadId = new LONG;
		*gt_pNUniqueThreadId = g_nUniqueThreadId;
		TlsSetValue(g_dwTlsID, gt_pNUniqueThreadId);

#ifdef DEBUG
		//�������
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
