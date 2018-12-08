#include "acg_net_lib.h"
#include "ACGIOCP.h"

#include "../base/acgException.h"
#include "../base/acgLoging.h"

#include "ACGChannel.h"

namespace acg
{
	namespace net
	{

		ACGIOCP::ACGIOCP(ACGEventLoop *loop)
			:ACGIO(loop)
		{
			m_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
		}


		ACGIOCP::~ACGIOCP()
		{

		}
		acg::base::CACGTimestamp ACGIOCP::Poll(DWORD timeoutMs, ChannelList * activeChannels)
		{
			/*
			try
			{
				OVERLAPPED_ENTRY *pOv = new OVERLAPPED_ENTRY[10000];
				ULONG num = 0;
				BOOL bRet = GetQueuedCompletionStatusEx(m_IOCP, pOv, 10000, &num, timeoutMs, FALSE);
				DWORD error = GetLastError();
				acg::base::CACGTimestamp now(acg::base::CACGTimestamp::now());
				if (bRet)
				{//成功
					if (num > 0)
					{
						LOG_DEBUG << L"得到" << num << L"个通知";

						for (ULONG i = 0; i < num; i++)
						{
							ACGOVERLAPPED *myOV = CONTAINING_RECORD(pOv[i].lpOverlapped, ACGOVERLAPPED, m_wsaol);
							ChannelMap::iterator ite = m_channels.find((HANDLE)myOV->m_socket), end = m_channels.end();
							ACG_ASSERT(ite != end);
							activeChannels->push_back(ite->second);
						}
					}
					else if(num == 0)
					{
						LOG_DEBUG << L"没有获得任何通知";
					}
					else
					{
						throw acg::base::CACGException(error);
					}
				}
				else
				{
					if (error != WAIT_TIMEOUT)
					{ //失败，抛出异常
						throw acg::base::CACGException(error);
					}
				}

				return now;
			}
			catch (acg::base::CACGException e)
			{
				LOG_FATAL << L"GetQueuedCompletionStatusEx 失败，失败原因：" << e.GetReason();
			}
			*/

			return acg::base::CACGTimestamp();

		}
		void ACGIOCP::UpdateChannel(ACGChannel * channel)
		{

		}
		void ACGIOCP::RemoveChannel(ACGChannel * channel)
		{

		}
	}
}
