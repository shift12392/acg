#pragma once

#include "acgNoCopyable.h"

#include <functional>
#include <vector>

#include "acgException.h"
#include "acg_DbgOut.h"
#include "acgThread.h"




namespace acg
{
    namespace base
    {
        class acgIOCP :public CACGNoCopyable
        {
        private:
            HANDLE  m_hIOCP;
            DWORD   m_dwConcurrent;               //最大并发线程数
            DWORD   m_dwThreadCnt;                //线程池中线程数
            std::vector<CACGThread*> m_vThread;



            DWORD   m_dwWaitTime;                 //线程池函数调用GetQueuedCompletionStatus函数等待的超时值
                                                  //默认值是INFINITE


            BOOL    m_bCreate = FALSE;

        public:

            typedef std::function<void()> CreateIOCPFunc;

            acgIOCP();
            ~acgIOCP();



        public:
            BOOL Create(VOID *pUserData, DWORD dwConcurrent = 0, DWORD dwThreadCnt = 0, LPSECURITY_ATTRIBUTES lpSAThread = NULL);


        private:
            //注：这个函数，线程创建失败会直接杀死进程。
            void BeginThreadPool(LPSECURITY_ATTRIBUTES lpsa = NULL);
            
            //线程池函数
            void ThreadPoolFunc();

        private:
            CreateIOCPFunc  m_onCreateIOCPFunc;

        };
    }

}

