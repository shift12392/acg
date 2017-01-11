

#pragma once

#include "acgNoCopyable.h"
#include "acgMutexLock.h"

namespace acg
{
	namespace base
	{
		class CACGCondition :public CACGNoCopyable
		{
		public:
			explicit CACGCondition(CACGMutexLock &cs) :m_cs(cs) {}
			~CACGCondition() {}

			void Wait()
			{
				if (!SleepConditionVariableCS(&m_condition, m_cs.GetCS(), INFINITE))
				{
					exit(0);
				}
			}

			BOOL WaitForTime(DWORD dwMilliseconds)
			{
				return SleepConditionVariableCS(&m_condition, m_cs.GetCS(), dwMilliseconds);
			}

			void Notify()
			{
				WakeConditionVariable(&m_condition);
			}

			void NotifyAll()
			{
				WakeAllConditionVariable(&m_condition);
			}

		private:
			CACGMutexLock    &m_cs;
			CONDITION_VARIABLE  m_condition;
		};
	}
}