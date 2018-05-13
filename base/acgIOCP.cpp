#include "acg_base_lib.h"
#include "acgIOCP.h"


#include "acgCountDownLatch.h"
#include "acgLoging.h"


namespace acg
{
    namespace base
    {


        namespace
        {
            CWTCountDownLatch *g_pDown=NULL;
        }



        acgIOCP::acgIOCP()
            :m_hIOCP(NULL)
            , m_dwConcurrent(0)
            , m_dwThreadCnt(0)
            , m_dwWaitTime(INFINITE)
            , m_bCreate(FALSE)
        {


        }


        acgIOCP::~acgIOCP()
        {
        }

        BOOL acgIOCP::Create(VOID *pUserData, DWORD dwConcurrent, DWORD dwThreadCnt, LPSECURITY_ATTRIBUTES lpSAThread )
        {
            BOOL bRet = FALSE;

            try
            {
                ACG_ASSERT(!m_bCreate);

                m_dwConcurrent = (0 == dwConcurrent) ? 1 : dwConcurrent;
                m_dwThreadCnt = (0 == dwThreadCnt) ? 2 : dwThreadCnt;
                m_dwConcurrent = min(m_dwConcurrent, m_dwThreadCnt);
                m_dwThreadCnt = max(m_dwConcurrent, m_dwThreadCnt);

                //创建IOCP对象
                m_hIOCP=CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)this, m_dwConcurrent);
                if (NULL != m_hIOCP)
                {
                    throw CACGException(GetLastError());
                }

                g_pDown = new CWTCountDownLatch(m_dwThreadCnt);
                //创建线程池
                BeginThreadPool(lpSAThread);

                //等待线程们都启动了
                g_pDown->Wait();

                
                //创建IOCP成功
                m_onCreateIOCPFunc();



            }
            catch (CACGException e)
            {


            }

        }


        void acgIOCP::BeginThreadPool(LPSECURITY_ATTRIBUTES lpsa)
        {
            try
            {
                for (DWORD i = 0; i < m_dwThreadCnt; i++)
                {
                    CACGThread *pIOCPThread=new CACGThread(std::bind(&acgIOCP::ThreadPoolFunc,this));
                    if (pIOCPThread->Start(lpsa))
                    {
                        throw CACGException(GetLastError());
                    }

                    m_vThread.push_back(pIOCPThread);
                }
            }
            catch (CACGException e)
            {
                LOG_SYSFATAL << L"IOCP线程池创建失败，失败原因：" << e.GetReason();
                exit(1);
            }
        }

        void acgIOCP::ThreadPoolFunc()
        {
            try
            {
                g_pDown->CountDown();

                BOOL bRet = 0;
                DWORD dwTransBytes = 0;
                ULONG_PTR key = NULL;
                OVERLAPPED *pOL = NULL;
                BOOL bLoop = TRUE;


                bRet=GetQueuedCompletionStatus(m_hIOCP,
                    &dwTransBytes,
                    &key,
                    &pOL,
                    m_dwWaitTime);
                if (bRet)
                {

                }

            }
            catch (CACGException e)
            {
               
            }
        }


    }
}
