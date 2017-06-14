

#pragma once

#include "acgNoCopyable.h"
#include "acgCondition.h"
#include "acgMutexLock.h"

namespace acg
{
	namespace base
	{
		class CWTCountDownLatch :public CACGNoCopyable
		{
		public:
			explicit CWTCountDownLatch(DWORD dwCount) 
				:m_condition(m_cs)
				,m_dwCount(dwCount)
			{

			}

			void Wait()
			{
				CACGMutexLockGuard lock(m_cs);
				while (m_dwCount > 0)
				{
					m_condition.Wait();
				}
			}

			void CountDown()
			{
				CACGMutexLockGuard lock(m_cs);
				--m_dwCount;
				if (m_dwCount == 0)
					m_condition.NotifyAll();
			}

			DWORD GetCount() const
			{
				CACGMutexLockGuard lock(m_cs);
				return m_dwCount;
			}

		private:
			DWORD m_dwCount;
			mutable CACGMutexLock m_cs;
			CACGCondition     m_condition;
		};
	}
}
