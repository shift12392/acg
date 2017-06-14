

#pragma once

#include "acg_UniqueThreadId.h"
#include "acgCurThread.h"
#include "acg_DbgOut.h"
#include "acgNoCopyable.h"

namespace acg
{
	namespace base
	{
		class  CACGMutexLock :public CACGNoCopyable 
		{
		public:
			CACGMutexLock() :m_nHolder(0)
			{
				InitializeCriticalSection(&m_cs);
			}
			~CACGMutexLock()
			{
				ACG_ASSERT(m_nHolder == 0);
				DeleteCriticalSection(&m_cs);
			}

            BOOL IsLockedByThisThread() const
            {
                return m_nHolder == CurThread::GetTid();
            }

            

		public:
			void Lock()
			{
				ACG_ASSERT(m_nHolder != CurThread::GetCurUniqueThreadId());
				EnterCriticalSection(&m_cs);
				m_nHolder = CurThread::GetCurUniqueThreadId();
			}

			void Unlock()
			{
				m_nHolder=0;
				LeaveCriticalSection(&m_cs);
			}

			//这个方法只能CACGCondition调用
			CRITICAL_SECTION* GetCS()
			{
				return &m_cs;
			}

		private:
			CRITICAL_SECTION m_cs;
			LONG             m_nHolder;

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
