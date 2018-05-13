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
            DWORD   m_dwConcurrent;               //��󲢷��߳���
            DWORD   m_dwThreadCnt;                //�̳߳����߳���
            std::vector<CACGThread*> m_vThread;



            DWORD   m_dwWaitTime;                 //�̳߳غ�������GetQueuedCompletionStatus�����ȴ��ĳ�ʱֵ
                                                  //Ĭ��ֵ��INFINITE


            BOOL    m_bCreate = FALSE;

        public:

            typedef std::function<void()> CreateIOCPFunc;

            acgIOCP();
            ~acgIOCP();



        public:
            BOOL Create(VOID *pUserData, DWORD dwConcurrent = 0, DWORD dwThreadCnt = 0, LPSECURITY_ATTRIBUTES lpSAThread = NULL);


        private:
            //ע������������̴߳���ʧ�ܻ�ֱ��ɱ�����̡�
            void BeginThreadPool(LPSECURITY_ATTRIBUTES lpsa = NULL);
            
            //�̳߳غ���
            void ThreadPoolFunc();

        private:
            CreateIOCPFunc  m_onCreateIOCPFunc;

        };
    }

}

