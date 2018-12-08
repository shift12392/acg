#pragma once


#include "ACGIO.h"

namespace acg
{
	namespace net
	{
		class ACGIOCP : public ACGIO
		{
		public:
			ACGIOCP(ACGEventLoop *loop);
			virtual ~ACGIOCP();

		private:
			HANDLE m_IOCP;

		public:
			virtual acg::base::CACGTimestamp Poll(DWORD timeoutMs, ChannelList* activeChannels);

			virtual void UpdateChannel(ACGChannel *channel);

			virtual void RemoveChannel(ACGChannel *channel);

		};
	}
}

