

#pragma once

#include "acg_UniqueThreadId.h"
#include "acg_DbgOut.h"
#include "acgNoCopyable.h"

namespace acg
{
	namespace base
	{
		class  CACGMutexLock :public CACGNoCopyable 
		{
		public:
#ifdef DEBUG
			CACGMutexLock() :m_nHolder(0)
			{
				InitializeCriticalSection(&m_cs);
			}
#else
			CACGMutexLock()
			{
				InitializeCriticalSection(&m_cs);
			}
#endif
			~CACGMutexLock()
			{
				ACG_ASSERT(m_nHolder == 0);
				DeleteCriticalSection(&m_cs);
			}

		public:
			void Lock()
			{
				ACG_ASSERT(m_nHolder != CurThread::GetCurUniqueThreadId());
				EnterCriticalSection(&m_cs);
#ifdef DEBUG
				m_nHolder = CurThread::GetCurUniqueThreadId();
#endif // DEBUG
			}

			void Unlock()
			{
#ifdef DEBUG
				m_nHolder=0;
#endif
				LeaveCriticalSection(&m_cs);
			}

			//这个方法只能CACGCondition调用
			CRITICAL_SECTION* GetCS()
			{
				return &m_cs;
			}

		private:
			CRITICAL_SECTION m_cs;
#ifdef DEBUG
			LONG             m_nHolder;
#endif // DEBUG

		};


		class CACGMutexLockGuard :public CACGNoCopyable
		{
		public:
			explicit CACGMutexLockGuard(CACGMutexLock &mutex) :m_mutex(mutex)
			{
				m_mutex.Lock();
			}
			~CACGMutexLockGuard()
			{
				m_mutex.Unlock();
			}
		private:
			CACGMutexLock  &m_mutex;
		};

	}
}

//为了防止用户粗心大意写成CACGMutexLockGuard()
#define CACGMutexLockGuard(x) ACG_ASSERT(FALSE)    
