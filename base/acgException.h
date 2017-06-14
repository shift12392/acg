



#pragma once


#ifndef ACG_BASE_ACGEXCEPTION_H
#define ACG_BASE_ACGEXCEPTION_H

#include "acg_base_lib.h"
#include <eh.h>
#include "acg_dll.h"

ACG_DLL extern int a;

namespace acg
{
	namespace base
	{

		namespace { const int g_ErrBuf_MaxLen = 4096; }
		class ACG_DLL CACGException 
		{
			enum EM_ACG_EXCEPTION
			{//异常的具体种类的枚举值，用于具体区分详细的异常信息
				ET_Empty = 0x0,
				ET_SE,
				ET_LastError,
				ET_Customer
			};

		protected:
			EM_ACG_EXCEPTION m_EType;		//异常类型

			UINT m_nSEHCode;                //SEH异常代码
			EXCEPTION_POINTERS *m_EP;

			DWORD m_dwLastError;            //错误代码

			LPWSTR m_lpErrMsg;              //错误描述

		public:
			CACGException() :m_nSEHCode(0), m_EP(NULL), m_dwLastError(0), m_lpErrMsg(0), m_EType(ET_Empty) {}
			CACGException(UINT nCode, EXCEPTION_POINTERS *ep);
			CACGException(DWORD dwLastError);
			CACGException(EM_ACG_EXCEPTION EType, LPCTSTR pszFmt, ...);
			CACGException(LPCTSTR pszFmt, ...);

		public:
			LPCWSTR GetReason() const
			{
				return m_lpErrMsg;
			}
			DWORD GetErrCode() const
			{
				return (0 == m_dwLastError) ? m_nSEHCode : m_dwLastError;
			}
			EM_ACG_EXCEPTION GetType()
			{
				return m_EType;
			}
			LPCWSTR GetTypeString()
			{
				static LPTSTR pszTypeString[] = {
					_T("Empty"),
					_T("SE"),
					_T("Thread Last Error"),
					_T("Customer"),
					NULL
				};

				if (m_EType < ACG_COUNTOF(pszTypeString))
				{
					return pszTypeString[m_EType];
				}
				return NULL;
			}
		public:
			//工具方法
			LPWSTR GetErrorString(DWORD dwErrorCode);
		};
	}
}


#endif // !ACG_BASE_ACGEXCEPTION_H



