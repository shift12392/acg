#pragma once

#include <vector>
#include <map>

#include "../base/acgNoCopyable.h"
#include "../base/acgTimestamp.h"
#include "acgEventLoop.h"

namespace acg
{
	namespace net
	{

		class ACGChannel;

		class ACGIO : public acg::base::CACGNoCopyable
		{
		public:
			typedef std::vector<ACGChannel*>      ChannelList;
			typedef std::map<HANDLE, ACGChannel*> ChannelMap;

			ACGIO(ACGEventLoop *loop);
			virtual ~ACGIO();

			/// Polls the I/O events.
			/// Must be called in the loop thread.
			virtual acg::base::CACGTimestamp Poll(int timeoutMs, ChannelList* activeChannels) = 0;

			virtual void UpdateChannel(ACGChannel *channel) = 0;

			virtual void RemoveChannel(ACGChannel *channel) = 0;

			virtual bool HasChannel(ACGChannel *channel) const;

			void AssertInLoopThread() const { return m_pLoop->AssertInLoopThread(); }

		protected:
			ACGEventLoop *m_pLoop;
			ChannelMap   m_channels;

		};
	}
}


