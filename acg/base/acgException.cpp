

#include "../stdafx.h"
#define ACG_EXPORT                //在这也要定义ACG_EXPORT
#include "acgException.h"
#include "acg_mem.h"

namespace acg
{
	namespace base
	{



		CACGException::CACGException(UINT nCode, EXCEPTION_POINTERS *ep):m_nSEHCode(nCode), m_EP(ep), m_dwLastError(0), m_lpErrMsg(0), m_EType(ET_SE)
		{//结构化异常
			m_lpErrMsg = (LPTSTR)ACG_CALLOC(g_ErrBuf_MaxLen * sizeof(TCHAR));
			//以下开关语句中的常量定义在ntstatus.h中 
			switch (m_nSEHCode)
			{
			case 0xC0000029://STATUS_INVALID_UNWIND_TARGET:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("展开(unwind)操作中遭遇无效的展开位置"));
			}
			break;
			case EXCEPTION_ACCESS_VIOLATION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("线程尝试对虚地址的一次无效访问."));
			}
			break;
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("线程尝试对有硬件数组边界检查的一次越界访问."));
			}
			break;
			case EXCEPTION_BREAKPOINT:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("遇到断点."));
			}
			break;
			case EXCEPTION_DATATYPE_MISALIGNMENT:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("线程尝试在不支持对齐的硬件上访问未对齐的数据."));
			}
			break;
			case EXCEPTION_FLT_DENORMAL_OPERAND:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("浮点数操作数太小."));
			}
			break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("浮点数被零除."));
			}
			break;
			case EXCEPTION_FLT_INEXACT_RESULT:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("浮点数结果无法被正确描述."));
			}
			break;
			case EXCEPTION_FLT_INVALID_OPERATION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("未知的浮点数异常."));
			}
			break;
			case EXCEPTION_FLT_OVERFLOW:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("浮点数操作中指数高于指定类型允许的数量级."));
			}
			break;
			case EXCEPTION_FLT_STACK_CHECK:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("浮点数操作中导致的栈溢出."));
			}
			break;
			case EXCEPTION_FLT_UNDERFLOW:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("浮点数操作中指数低于指定类型需要的数量级."));
			}
			break;
			case EXCEPTION_GUARD_PAGE:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("线程访问到被PAGE_GUARD修饰分配的内存."));
			}
			break;
			case EXCEPTION_ILLEGAL_INSTRUCTION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("线程试图执行不可用指令."));
			}
			break;
			case EXCEPTION_IN_PAGE_ERROR:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("线程试图访问一个系统当前无法载入的缺失页."));
			}
			break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("整数被零除."));
			}
			break;
			case EXCEPTION_INT_OVERFLOW:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("整数操作结果进位超出最高有效位."));
			}
			break;
			case EXCEPTION_INVALID_DISPOSITION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("异常处理过程返回了一个不可用控点给异常发分发过程."));
			}
			break;
			case EXCEPTION_INVALID_HANDLE:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("线程使用了一个不可用的内核对象句柄(该句柄可能已关闭)."));
			}
			break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("线程试图在不可继续的异常之后继续执行."));
			}
			break;
			case EXCEPTION_PRIV_INSTRUCTION:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("线程试图通过执行一条指令，完成一个不允许在当前计算机模式的操作."));
			}
			break;
			case EXCEPTION_SINGLE_STEP:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("某指令执行完毕信号, 可能来自一个跟踪陷阱或其他单指令机制."));
			}
			break;
			case EXCEPTION_STACK_OVERFLOW:
			{
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("线程栈空间耗尽."));
			}
			break;
			default://未处理的异常！
				StringCchPrintf(m_lpErrMsg, g_ErrBuf_MaxLen, _T("CACGException类未知异常,code=0x%08X"),
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
			//工具方法帮助得到一个错误代码的错误信息 
            //注意因为这个方法申请了对象内部的字符串内存，
            //为了能够在对象失效时正常释放所以没有把这个方法设计成静态方法
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