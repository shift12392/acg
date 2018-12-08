

#include "acg_net_lib.h"
#include "ACGChannel.h"


const SHORT acg::net::ACGChannel::sNoneEvent = 0;
const SHORT acg::net::ACGChannel::sReadEvent = POLLIN;
const SHORT acg::net::ACGChannel::sWriteEvent = POLLOUT;

acg::net::ACGChannel::ACGChannel(ACGEventLoop * loop, HANDLE handle)
	: m_loop(loop)
	, m_hanle(handle)
	, m_index(0)
{
	//m_ov.m_socket = (SOCKET)handle;
	//m_ov.m_wsaol = {};
}

acg::net::ACGChannel::~ACGChannel()
{
}

void acg::net::ACGChannel::HandleEvent(acg::base::CACGTimestamp receiveTime)
{
}

void acg::net::ACGChannel::Update()
{
}
