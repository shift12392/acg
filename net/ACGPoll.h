#pragma once

#include "ACGIO.h"

namespace acg
{
	namespace net
	{

		class ACGPoll : public ACGIO
		{
		public:
			ACGPoll(ACGEventLoop *loop);
			virtual ~ACGPoll();

		public:
			virtual acg::base::CACGTimestamp Poll(int timeoutMs, ChannelList* activeChannels);
			virtual void UpdateChannel(ACGChannel *channel);
			virtual void RemoveChannel(ACGChannel *channel);


		private:

			void FillActiveChannels(int numEvents, ChannelList* activeChannels);

			//C++��֤vector���ݰ���˳�򴢴�
			typedef std::vector<WSAPOLLFD> PollFdList;

			PollFdList m_pollFds;

		};
	}
}

