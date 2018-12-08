



#include "acg_net_lib.h"
#include "ACGIO.h"

#include "ACGChannel.h"

namespace acg
{
	namespace net
	{

		ACGIO::ACGIO(ACGEventLoop *loop)
			:m_pLoop(loop)
		{
		}


		ACGIO::~ACGIO()
		{
		}

		bool ACGIO::HasChannel(ACGChannel * channel) const
		{
			AssertInLoopThread();
			ChannelMap::const_iterator ite = m_channels.find(channel->GetHandle());
			return ite != m_channels.end() && ite->second == channel;
		}

	}
}
