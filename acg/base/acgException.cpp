

#include "../stdafx.h"
#define ACG_EXPORT                //����ҲҪ����ACG_EXPORT
#include "acgException.h"
#include "acg_mem.h"

namespace acg
{
	namespace base
	{



		CACGException::CACGException(UINT nCode, EXCEPTION_POINTERS *ep):m_nSEHCode(nCode), m_EP(ep), m_dwLastError(0), m_lpErrMsg(0), m_EType(ET_SE)
		{//�ṹ���쳣
			m_lpErrMsg = (LPTSTR)ACG_CALLOC(g_ErrBuf_MaxLen * sizeof(TCHAR));
			//���¿�������еĳ���������ntstatus.h�� 
			switch (m_nSEHCode)
			{
			case 0xC0000029://STATUS_INVALID_UNWIND_TARGET:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("չ��(unwind)������������Ч��չ��λ��"));
			}
			break;
			case EXCEPTION_ACCESS_VIOLATION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�̳߳��Զ����ַ��һ����Ч����."));
			}
			break;
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�̳߳��Զ���Ӳ������߽����һ��Խ�����."));
			}
			break;
			case EXCEPTION_BREAKPOINT:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�����ϵ�."));
			}
			break;
			case EXCEPTION_DATATYPE_MISALIGNMENT:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�̳߳����ڲ�֧�ֶ����Ӳ���Ϸ���δ���������."));
			}
			break;
			case EXCEPTION_FLT_DENORMAL_OPERAND:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("������������̫С."));
			}
			break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�����������."));
			}
			break;
			case EXCEPTION_FLT_INEXACT_RESULT:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("����������޷�����ȷ����."));
			}
			break;
			case EXCEPTION_FLT_INVALID_OPERATION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("δ֪�ĸ������쳣."));
			}
			break;
			case EXCEPTION_FLT_OVERFLOW:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("������������ָ������ָ�����������������."));
			}
			break;
			case EXCEPTION_FLT_STACK_CHECK:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�����������е��µ�ջ���."));
			}
			break;
			case EXCEPTION_FLT_UNDERFLOW:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("������������ָ������ָ��������Ҫ��������."));
			}
			break;
			case EXCEPTION_GUARD_PAGE:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�̷߳��ʵ���PAGE_GUARD���η�����ڴ�."));
			}
			break;
			case EXCEPTION_ILLEGAL_INSTRUCTION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�߳���ͼִ�в�����ָ��."));
			}
			break;
			case EXCEPTION_IN_PAGE_ERROR:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�߳���ͼ����һ��ϵͳ��ǰ�޷������ȱʧҳ."));
			}
			break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("���������."));
			}
			break;
			case EXCEPTION_INT_OVERFLOW:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�������������λ���������Чλ."));
			}
			break;
			case EXCEPTION_INVALID_DISPOSITION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�쳣������̷�����һ�������ÿص���쳣���ַ�����."));
			}
			break;
			case EXCEPTION_INVALID_HANDLE:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�߳�ʹ����һ�������õ��ں˶�����(�þ�������ѹر�)."));
			}
			break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�߳���ͼ�ڲ��ɼ������쳣֮�����ִ��."));
			}
			break;
			case EXCEPTION_PRIV_INSTRUCTION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�߳���ͼͨ��ִ��һ��ָ����һ���������ڵ�ǰ�����ģʽ�Ĳ���."));
			}
			break;
			case EXCEPTION_SINGLE_STEP:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("ĳָ��ִ������ź�, ��������һ�����������������ָ�����."));
			}
			break;
			case EXCEPTION_STACK_OVERFLOW:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("�߳�ջ�ռ�ľ�."));
			}
			break;
			default://δ������쳣��
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("CACGException��δ֪�쳣,code=0x%08X"),
					m_nSEHCode);
				break;
			};
		}

		CACGException::CACGException(DWORD dwLastError)
			: m_nSEHCode(0), m_EP(NULL), m_dwLastError(dwLastError), m_lpErrMsg(NULL), m_EType(ET_LastError)
		{
			GetErrorString(dwLastError);
		}

		CACGException::CACGException(EM_ACG_EXCEPTION EType, LPCTSTR pszFmt, ...)
			: m_nSEHCode(0), m_EP(NULL), m_dwLastError(0), m_lpErrMsg(NULL), m_EType(EType)
		{
			va_list va;
			va_start(va, pszFmt);
			m_lpErrMsg = (LPTSTR)ACG_CALLOC(g_ErrBuf_MaxLen * sizeof(TCHAR));
			if (NULL != m_lpErrMsg)
			{
				StringCchVPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, pszFmt, va);
			}
			va_end(va);
		}

		CACGException::CACGException(LPCTSTR pszFmt, ...)
			: m_nSEHCode(0), m_EP(NULL), m_dwLastError(0), m_lpErrMsg(NULL), m_EType(ET_Customer)
		{
			va_list va;
			va_start(va, pszFmt);

			m_lpErrMsg = (LPTSTR)ACG_CALLOC(g_ErrBuf_MaxLen * sizeof(TCHAR));
			if (NULL != m_lpErrMsg)
			{
				StringCchVPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, pszFmt, va);
			}
			va_end(va);
		}

		LPTSTR CACGException::GetErrorString(DWORD dwErrorCode)
		{
			//���߷��������õ�һ���������Ĵ�����Ϣ 
            //ע����Ϊ������������˶����ڲ����ַ����ڴ棬
            //Ϊ���ܹ��ڶ���ʧЧʱ�����ͷ�����û�а����������Ƴɾ�̬����
			ACG_SAFEFREE(m_lpErrMsg);
			LPTSTR lpMsgBuf = NULL;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
			if (NULL == lpMsgBuf)
			{
				return NULL;
			}
			size_t szLen = 0;
			StringCchLength(lpMsgBuf, g_ErrBuf_MaxLen, &szLen);
			if (szLen > 0)
			{
				m_lpErrMsg = (LPTSTR)ACG_CALLOC((szLen + 1) * sizeof(TCHAR));
				if (NULL != m_lpErrMsg)
				{
					StringCchCopy((LPTSTR)m_lpErrMsg, (szLen + 1), lpMsgBuf);
				}
				LocalFree(lpMsgBuf);
			}
			return m_lpErrMsg;
		}

	}
}