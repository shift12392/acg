#pragma once

#include "../base/acgNoCopyable.h"

namespace acg
{
    namespace net
    {
        class CACGSocket :public base::CACGNoCopyable
        {
        public:
            explicit CACGSocket(SOCKET socket);
            ~CACGSocket();


		public:
			static void Bind(SOCKET handle, const struct sockaddr* addr);
			static void FromIpPort(const WCHAR* ip, USHORT port, struct sockaddr_in *addr);
			static void FromIp6Port(const WCHAR* ip, USHORT port, struct sockaddr_in6 *addr6);

			static void ToIp(WCHAR *ip, size_t length,const SOCKADDR *sockaddr);
			static void ToIp(WCHAR *ip, size_t length, const SOCKADDR_IN *sockaddr);
			static void ToIp(WCHAR *ip, size_t length, const SOCKADDR_IN6 *sockaddr);
			static void ToIpPort(WCHAR *ip, size_t length, const SOCKADDR *sockaddr);
			static void ToIpPort(WCHAR *ip, size_t length, const SOCKADDR_IN *sockaddr);
			static void ToIpPort(WCHAR *ip, size_t length, const SOCKADDR_IN6 *sockaddr);

			static const SOCKADDR_IN* sockaddr_in_cast(const SOCKADDR* sockaddr);
			static const SOCKADDR_IN6* sockaddr_in6_cast(const SOCKADDR* sockaddr);
			static const SOCKADDR* sockaddr_cast(const SOCKADDR_IN6* sockaddr_in6);

        private:
            const SOCKET m_socket;
        };
    }
}