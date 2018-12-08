#pragma once

#include "../base/acgStringPiece.h"
#include "../base/acg_type.h"

#include "acgSocket.h"

namespace acg
{
    namespace net
    {
        class ACGInetAddress
        {
		public:

			//创建listen用地址
			explicit ACGInetAddress(USHORT port = 0, bool bIsLoopBack = false, bool ipv6 = false);

			ACGInetAddress(acg::base::CACGStringArg ip, USHORT port, bool ipv6 = false);

			explicit ACGInetAddress(const SOCKADDR_IN& addr_in)
				:m_addr(addr_in)
			{

			}

			explicit ACGInetAddress(const SOCKADDR_IN6& addr_in6)
				:m_addr6(addr_in6)
			{

			}

			~ACGInetAddress();


			const SOCKADDR* GetSockAddr() const { return CACGSocket::sockaddr_cast(&m_addr6); }

			acg::base::CACGString ToIp() const;
			acg::base::CACGString ToIpPort() const;

		private:
			union
			{
				struct sockaddr_in m_addr;
				struct sockaddr_in6 m_addr6;
			};
        };
    }
}
