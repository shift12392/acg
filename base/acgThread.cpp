#include "acg_base_lib.h"
#include "acgThread.h"
#include "acgCurThread.h"
#include "acg_DbgOut.h"
#include "acgException.h"



#include <process.h>



namespace acg
{


	namespace detail
	{

		struct CThreadData
		{
			typedef base::CACGThread::ThreadFunc ThreadFunc;
			ThreadFunc     m_threadFunc;
			acg::base::CACGString    m_strName;
			std::weak_ptr<LONG>   m_pThreadTid;
			CThreadData(const ThreadFunc& func, acg::base::CACGString &name, const std::shared_ptr<LONG>& tid)
				:m_threadFunc(func),
				m_strName(name),
				m_pThreadTid(tid)
			{ }

			void RunInThread()
			{
				LONG tid = base::CurThread::GetTid();
				std::shared_ptr<LONG>  pTid = m_pThreadTid.lock();       //如果提升失败，说明创建本线程的线程类已经析构了。
				if (pTid)           
				{
					*pTid = tid;
					pTid.reset();
				}

				base::CurThread::pszThreadName = m_strName.empty() ? L"acgThread" : m_strName.c_str();

				try
				{
					m_threadFunc();
					base::CurThread::pszThreadName = L"finished";
				}
				catch (base::CACGException e)
				{
					ACG_ASSERT(FALSE);
                    exit(1);
				}
			}

		};

		unsigned ACG_STDCL StartThread(void * obj)
		{
			CThreadData* pData = static_cast<CThreadData*>(obj);
			pData->RunInThread();
			delete pData;
			return 0;
		}
	}


	namespace base
	{

		namespace CurThread
		{
			ACG_TLS LONG gt_nThreadId =0;
			ACG_TLS WCHAR gt_szThreadId[64] = {};
			ACG_TLS size_t gt_nThreadIdLength=6;
			ACG_TLS LPCWSTR pszThreadName=L"unknown";

			void CacheThreadId()
			{
				if (0 == gt_nThreadId)
				{
                    gt_nThreadId = GetCurUniqueThreadId();
					ACG_CHECK_STR(StringCchPrintfW(gt_szThreadId, ACG_COUNTOF(gt_szThreadId), L"%5d", gt_nThreadId));
					ACG_CHECK_STR(StringCchLengthW(gt_szThreadId, ACG_COUNTOF(gt_szThreadId), (size_t*)(&gt_nThreadIdLength)));
				}
			}

		}

	
		class CThreadNameInitializer
		{
		public:
			CThreadNameInitializer()
			{
				CurThread::pszThreadName = L"main";
				CurThread::GetTid();
			}
		};

		//注：线程局部储存区在c++全局对象初始化之前被设置。
		CThreadNameInitializer  g_initApp;

		CACGAtomic32 CACGThread::m_createNum;

		CACGThread::CACGThread(const ThreadFunc& func, const CACGString& name )
			:m_bStart(FALSE),
			m_bWait(FALSE),
			m_hThread(NULL),
			m_pThreadTid(new LONG(0)),
			m_threadFunc(func),
			m_strName(name)
		{
			SetDefName();
		}
		CACGThread::~CACGThread()
		{
			if (NULL!=m_hThread && m_bStart && !m_bWait)
			{
				::CloseHandle(m_hThread);
			}
		}


		void CACGThread::Start()
		{
			ACG_ASSERT(!m_bStart);
			m_bStart = TRUE;

			detail::CThreadData *pData = new detail::CThreadData(m_threadFunc, m_strName, m_pThreadTid);
			
			try
			{
				m_hThread = (HANDLE)_beginthreadex(NULL, 0, detail::StartThread, (void*)pData, 0, NULL);

				//创建线程失败
				if (NULL == reinterpret_cast<uintptr_t>(m_hThread)
					|| 0xFFFFFFFFFFFFFFFF == reinterpret_cast<uintptr_t>(m_hThread))
				{
					throw CACGException(GetLastError());
				}
			}
			catch (CACGException e)
			{
				ACG_ASSERT(FALSE);
				ACG_DBGOUT_EXPW(Start, e);
				abort();                    //线程创建失败
			}
		}


		void CACGThread::Wait()
		{
			ACG_ASSERT(FALSE);
		}

		void CACGThread::SetDefName()
		{
			unsigned int num = m_createNum.IncrementAndGet();
			if (m_strName.empty())
			{
				WCHAR szName[64] = {};
				::StringCchPrintfW(szName, ACG_COUNTOF(szName), L"Thread%d", num);
				m_strName = szName;
			}
		}






	}
}
