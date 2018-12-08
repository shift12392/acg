#include "acg_net_lib.h"
#include "ACGPoll.h"

#include "ACGChannel.h"

#include "../base/acgLoging.h"



namespace acg
{
	namespace net
	{

		ACGPoll::ACGPoll(ACGEventLoop *loop)
			:ACGIO(loop)
		{
		}


		ACGPoll::~ACGPoll()
		{
		}


		acg::base::CACGTimestamp ACGPoll::Poll(int timeoutMs, ChannelList * activeChannels)
		{
			int numEvents = WSAPoll(&*m_pollFds.begin(), static_cast<ULONG>(m_pollFds.size()), timeoutMs);
			acg::base::CACGTimestamp now(acg::base::CACGTimestamp::now());
			DWORD error = GetLastError();
			if (numEvents > 0)
			{
				FillActiveChannels(numEvents, activeChannels);
			}
			else if (numEvents == 0)
			{
				LOG_TRACE << L"nothing happened";
			}
			else
			{
				LOG_SYSFATAL << L"WSAPoll³ö´í£¬´íÎó´úÂë£º" << error;
			}

			return now;
		}
		void ACGPoll::UpdateChannel(ACGChannel * channel)
		{
			AssertInLoopThread();

		}
		void ACGPoll::RemoveChannel(ACGChannel * channel)
		{
			AssertInLoopThread();
		}
		void ACGPoll::FillActiveChannels(int numEvents, ChannelList * activeChannels)
		{
			for (PollFdList::const_iterator ite = m_pollFds.begin(); ite != m_pollFds.end() && numEvents > 0; ite++)
			{
				if (ite->revents > 0)
				{
					--numEvents;
					ChannelMap::iterator channelIte = m_channels.find((HANDLE)ite->fd);
					ACG_ASSERT(channelIte != m_channels.end());
					ACGChannel* channel = channelIte->second;
					ACG_ASSERT(channel->GetHandle() == (HANDLE)ite->fd);
					channel->SetREvent(ite->revents);
					activeChannels->push_back(channel);
				}
			}
		}
	}
}
