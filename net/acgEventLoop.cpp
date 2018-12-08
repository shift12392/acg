

#include "acg_net_lib.h"
#include "acgEventLoop.h"
#include "../base/acgLoging.h"

namespace acg
{
	namespace net
	{
		ACG_TLS ACGEventLoop *gt_loopInThisThread = nullptr;
	}
}

acg::net::ACGEventLoop::ACGEventLoop()
	:m_bIsLoop(false)
	, m_bIsQuiting(false)
	, m_threadId(acg::base::CurThread::GetTid())
{
	LOG_DEBUG << L"ACGEventLoop created " << this << L"in this thread " << m_threadId;
	if (gt_loopInThisThread != nullptr)
	{
		LOG_FATAL << L"Another ACGEventLoop " << gt_loopInThisThread << L"exisits in this thread " << m_threadId;
	}
	else
	{
		gt_loopInThisThread = this;
	}


}

acg::net::ACGEventLoop::~ACGEventLoop()
{
}

void acg::net::ACGEventLoop::Loop()
{
	ACG_ASSERT(!m_bIsLoop);
	AbortNoInLoopThread();
	m_bIsLoop = true;
	m_bIsQuiting = false;
	LOG_DEBUG << L"ACGEventLoop " << this << L"start looping.";

	while (!m_bIsQuiting)
	{

	}

}

void acg::net::ACGEventLoop::AbortNoInLoopThread()
{
	LOG_FATAL << L"EventLoop::abortNotInLoopThread - EventLoop " << this
		<< L" was created in threadId_ = " << m_threadId
		<< L", current thread id = " << acg::base::CurThread::GetTid();
}
