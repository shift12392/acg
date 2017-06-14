#pragma once


#include "acgAtomic.h"
#include "acgNoCopyable.h"
#include "acg_type.h"

#include <functional>
#include <memory>



namespace acg
{
	namespace base
	{
		class CACGThread : public CACGNoCopyable
		{
		public:
			typedef std::function<void()> ThreadFunc;

			explicit CACGThread(const ThreadFunc&, const CACGString& name = CACGString());
			~CACGThread();

            //�����߳�
			void Start();
            //�ȴ��߳̽���
			void Wait();
			

			const CACGString& GetName() const {	return m_strName;}
			LONG  GetTid() const { return *m_pThreadTid; }
			BOOL IsStarted() const { return m_bStart; }

			static int NumCreate() { return m_createNum.Get(); }

		private:

			void SetDefName();

			BOOL             m_bStart ;
			BOOL             m_bWait  ;
			HANDLE           m_hThread ;
			std::shared_ptr<LONG>   m_pThreadTid;    //���̵߳�ID
			CACGString      m_strName;              //���̵߳ĺ���
			ThreadFunc       m_threadFunc;

			static CACGAtomic32 m_createNum;

		};
	}
}


