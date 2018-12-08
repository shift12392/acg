

#include "acg_net_lib.h"
#include "acgInetAddress.h"
#include "../base/acg_DbgOut.h"


namespace acg
{
	namespace net
	{

		static_assert(sizeof(ACGInetAddress) == sizeof(struct sockaddr_in6), "ACGInetAddress is same size as sockaddr_in6");
		
		//offsetof,该宏用于求结构体中一个成员在该结构体中的偏移量
		static_assert(offsetof(sockaddr_in, sin_family) == 0, "sin_family offset 0");
		static_assert(offsetof(sockaddr_in6, sin6_family) == 0, "sin_family offset 0");
		static_assert(offsetof(sockaddr_in, sin_port) == 2, "sin_port offset 2");
		static_assert(offsetof(sockaddr_in6, sin6_port) == 2, "sin6_port offset 2");

		ACGInetAddress::ACGInetAddress(USHORT port , bool bIsLoopBack , bool ipv6 )
		{
			static_assert(offsetof(ACGInetAddress, m_addr) == 0, "m_addr offset 0");
			static_assert(offsetof(ACGInetAddress, m_addr6) == 0, "m_addr6 offset 0");

			if (ipv6)
			{
				ACG_ASSERT(FALSE);           //没有测试过下面的代码是否正确，测试没问题时删除此行

				ZeroMemory(&m_addr6, sizeof(m_addr6));
				m_addr6.sin6_family = AF_INET6;
				m_addr6.sin6_port = htons(port);
				m_addr6.sin6_addr = bIsLoopBack ? in6addr_loopback : in6addr_any;
			}
			else
			{
				ZeroMemory(&m_addr, sizeof(m_addr));
				m_addr.sin_family = AF_INET;
				m_addr.sin_addr.S_un.S_addr = htonl(bIsLoopBack ? INADDR_LOOPBACK : INADDR_ANY);
				m_addr.sin_port = htons(port);
			}

		}

		ACGInetAddress::ACGInetAddress(acg::base::CACGStringArg ip, USHORT port, bool ipv6)
		{
			if (ipv6)
			{
				ACG_ASSERT(FALSE);           //没有测试过下面的代码是否正确

				ZeroMemory(&m_addr6, sizeof(m_addr6));
				m_addr6.sin6_family = AF_INET6;
				CACGSocket::FromIp6Port(ip.c_str(), port, &m_addr6);
			}
			else
			{
				ZeroMemory(&m_addr, sizeof(m_addr));
				m_addr.sin_family = AF_INET;
				CACGSocket::FromIpPort(ip.c_str(), port, &m_addr);
			}
		}


		ACGInetAddress::~ACGInetAddress()
		{
		}

		acg::base::CACGString ACGInetAddress::ToIp() const
		{
			WCHAR szBuf[64] = L"";
			if (m_addr.sin_family == AF_INET)
			{
				CACGSocket::ToIp(szBuf, 64, &m_addr);
			}
			else if (m_addr.sin_family == AF_INET6)
			{
				CACGSocket::ToIp(szBuf, 64, &m_addr6);
			}

			return acg::base::CACGString(szBuf);
		}

		acg::base::CACGString ACGInetAddress::ToIpPort() const
		{
			WCHAR szBuf[64] = L"";
			if (m_addr.sin_family == AF_INET)
			{
				CACGSocket::ToIpPort(szBuf, 64, &m_addr);
			}
			else if (m_addr.sin_family == AF_INET6)
			{
				CACGSocket::ToIpPort(szBuf, 64, &m_addr6);
			}
			return acg::base::CACGString(szBuf);
		}
	}
}
