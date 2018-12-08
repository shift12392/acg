#pragma once

#include "../base/acgNoCopyable.h"
#include "../base/acgCurThread.h"


namespace acg
{
	namespace net
	{
		class ACGEventLoop : public acg::base::CACGNoCopyable
		{
		public:
			ACGEventLoop();
			~ACGEventLoop();

		private:
			bool m_bIsLoop;
			bool m_bIsQuiting;
			LONG m_threadId;

		public:
			void Loop();

			bool IsInLoopThread() { return m_threadId == acg::base::CurThread::GetTid(); }
			void AssertInLoopThread()
			{
				if (!IsInLoopThread())
				{
					AbortNoInLoopThread();
				}
			}

		private:
			void AbortNoInLoopThread();
		};
	}
}
