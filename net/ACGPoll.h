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

			//C++保证vector数据按照顺序储存
			typedef std::vector<WSAPOLLFD> PollFdList;

			PollFdList m_pollFds;

		};
	}
}

