#pragma once


#include "acgAtomic.h"
#include "acgNoCopyable.h"


#include <functional>
#include <memory>
#include <string>


namespace acg
{
	namespace base
	{
		class CACGThread : public CACGNoCopyable
		{
		public:
			typedef std::function<void()> ThreadFunc;

			explicit CACGThread(const ThreadFunc&, const std::string& name = std::string());
			~CACGThread();


			void Start();
			void Wait();
			

			const std::string& GetName(){	return m_strName;}
			LONG  GetTid() { return *m_pThreadTid; }
			BOOL IsStarted() { return m_bStart; }

			static int NumCreate() { return m_createNum.Get(); }

		private:

			void SetDefName();

			BOOL             m_bStart ;
			BOOL             m_bWait  ;
			HANDLE           m_hThread ;
			std::shared_ptr<LONG>   m_pThreadTid;
			std::string      m_strName;
			ThreadFunc       m_threadFunc;

			static CACGAtomic32 m_createNum;

		};
	}
}


