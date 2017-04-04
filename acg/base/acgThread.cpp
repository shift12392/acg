#include "../stdafx.h"
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
			std::string    m_strName;
			std::weak_ptr<LONG>   m_pThreadTid;
			CThreadData(const ThreadFunc& func, const std::string &name, const std::shared_ptr<LONG>& tid)
				:m_threadFunc(func),
				m_strName(name),
				m_pThreadTid(tid)
			{ }

			void RunInThread()
			{
				LONG tid = base::CurThread::GetTid();
				std::shared_ptr<LONG>  pTid = m_pThreadTid.lock();
				if (pTid)
				{
					*pTid = tid;
					pTid.reset();
				}

				base::CurThread::pszThreadName = m_strName.empty() ? "acgThread" : m_strName.c_str();

				try
				{
					m_threadFunc();
					base::CurThread::pszThreadName = "finished";
				}
				catch (base::CACGException e)
				{
					ACG_ASSERT(FALSE);
					abort();
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
			ACG_TLS LONG gt_dwThreadId=0;
			ACG_TLS char gt_szThreadId[64] = {};
			ACG_TLS size_t gt_nThreadIdLength=6;
			ACG_TLS LPCSTR pszThreadName="unknown";

			void CacheThreadId()
			{
				if (0 == gt_dwThreadId)
				{
					gt_dwThreadId = GetCurUniqueThreadId();
					ACG_CHECK_STR(StringCchPrintfA(gt_szThreadId, ACG_COUNTOF(gt_szThreadId), "%5d", gt_dwThreadId));
					ACG_CHECK_STR(StringCchLengthA(gt_szThreadId, ACG_COUNTOF(gt_szThreadId), (size_t*)(&gt_nThreadIdLength)));
				}
			}

		}

	
		class CThreadNameInitializer
		{
		public:
			CThreadNameInitializer()
			{
				CurThread::pszThreadName = "main";
				CurThread::GetTid();
			}
		};

		//ע���ֲ߳̾���������c++ȫ�ֶ����ʼ��֮ǰ�����á�
		CThreadNameInitializer  g_initApp;

		CACGAtomic32 CACGThread::m_createNum;

		CACGThread::CACGThread(const ThreadFunc& func, const std::string& name )
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

				//�����߳�ʧ��
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
				abort();                    //�̴߳���ʧ��
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
				char szName[64] = {};
				::StringCchPrintfA(szName, sizeof(szName), "Thread%d", num);
				m_strName = szName;
			}
		}






	}
}
