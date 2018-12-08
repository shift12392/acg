#include "acg_net_lib.h"
#include "acgSocket.h"

#include "../base/acgLoging.h"
#include "../base/acg_type.h"


namespace acg
{
    namespace net
    {
        CACGSocket::CACGSocket(SOCKET socket)
            :m_socket(socket)
        {
        }


        CACGSocket::~CACGSocket()
        {
        }

		void CACGSocket::Bind(SOCKET handle, const struct sockaddr* addr)
		{
			/*
			    在后面调用listen函数之前，在未连接的套接字上需要调用bind函数。 
				它通常用于绑定到面向连接（流）或无连接（数据报）的套接字。 
				bind函数也可用于绑定到原始套接字（套接字是通过调用socket函数创建的，类型参数设置为SOCK_RAW）。 
				对connect，ConnectEx，WSAConnect，WSAConnectByList或WSAConnectByName函数调用之前，绑定函数也可以在未连接的套接字上使用。

				当通过调用socket函数创建套接字时，它存在于命名空间（地址族）中，但没有为其分配名称。
				使用bind函数通过为未命名的套接字分配本地名称来建立套接字的本地关联。

				使用Internet地址族时，名称由三部分组成：
					地址族。
					主机地址。
					标识应用程序的端口号。

				在Windows套接字2中，name参数不会严格解释为指向sockaddr结构的指针。
				它以这种方式转换为Windows Sockets 1.1兼容性。
				Service providers可以将其视为指向namelen大小的内存块的指针。
				此块中的前2个字节（对应于sockaddr结构的sa_family成员，sockaddr_in结构的sin_family成员或sockaddr_in6结构的sin6_family成员）必须包含用于创建套接字的地址族。 
				否则，会发生错误WSAEFAULT。

				如果应用程序不关心分配了哪个本地地址，请为IPv4本地地址指定常量值INADDR_ANY，或者在name参数的sa_data成员中为IPv6本地地址指定常量值in6addr_any。 这允许底层服务提供者使用任何适当的网络地址，可能在存在多宿主主机（即具有多个网络接口和地址的主机）的情况下简化应用程序编程。

				对于TCP / IP，如果端口指定为零，则服务提供者从动态客户端端口范围为应用程序分配唯一端口。 在Windows Vista及更高版本中，动态客户端端口范围是介于49152和65535之间的值。这是对Windows Server 2003及更早版本的更改，其中动态客户端端口范围是介于1025和5000之间的值。客户端动态的最大值 可以通过在以下注册表项下设置值来更改端口范围：HKLM\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters
				MaxUserPort注册表值设置用于动态客户端端口范围的最大值的值。 您必须重新启动计算机才能使此设置生效。

				在Windows Vista及更高版本中，可以使用netsh命令查看和更改动态客户端端口范围。 可以为UDP和TCP以及IPv4和IPv6设置不同的动态客户端端口范围。 有关更多信息，请参见知识库文章929851。

				在调用bind之后，应用程序可以使用getsockname来得到已分配给套接字的地址和端口。 如果Internet地址等于INADDR_ANY或in6addr_any，则在the socket connected之前，getsockname不一定能提供地址，因为如果主机是多宿主的，则几个地址可能有效。 不建议客户端应用程序绑定到端口0以外的特定端口号，因为存在与已在本地计算机上使用该端口号的另一个套接字冲突的危险。

				注意：在使用带有SO_EXCLUSIVEADDRUSE或SO_REUSEADDR套接字选项的bind时，必须在执行bind之前设置套接字选项才能产生任何影响。 有关详细信息，请参阅SO_EXCLUSIVEADDRUSE和使用SO_REUSEADDR和SO_EXCLUSIVEADDRUSE。

			*/
			if (0 != bind(handle, addr, sizeof(struct sockaddr)))
			{
				LOG_SYSFATAL << L"绑定失败";
			}
		}
		void CACGSocket::FromIpPort(const WCHAR * ip, USHORT port, sockaddr_in * addr)
		{
			IN_ADDR in_addr;
			ZeroMemory(&in_addr, sizeof(in_addr));
			int nRet = InetPtonW(AF_INET, ip, &in_addr);

			if (nRet == 0)
			{
				LOG_SYSERR << L"ip地址字符串拼写错误：" << ip;
			}
			else if (nRet != 1)
			{
				LOG_SYSERR << L"从ip地址字符串到二进制出错";
			}

			addr->sin_addr.S_un.S_addr = in_addr.S_un.S_addr;
			addr->sin_port = htons(port);
		}
		void CACGSocket::FromIp6Port(const WCHAR * ip, USHORT port, sockaddr_in6 * addr6)
		{
			//InetPton函数将标准文本表示形式的IPv4或IPv6 Internet网络地址转换为其数字二进制形式。 此函数的ANSI版本为inet_pton。
			//@Family：地址族的可能值在Ws2def.h头文件中定义。 请注意，Ws2def.h头文件自动包含在Winsock2.h中，不应直接使用。 请注意，AF_地址族和PF_协议族常量的值是相同的（例如，AF_INET和PF_INET），因此可以使用常量。
			//         目前仅支持AF_INET和AF_INET6
			//@pszAddrString :指向以NULL结尾的字符串的指针，该字符串包含要转换为数字二进制形式的IP地址的文本表示形式。
			//                当Family参数为AF_INET时，pszAddrString参数必须以标准点分十进制表示法指向IPv4地址的文本表示形式。
			//	              当Family参数为AF_INET6时，pszAddrString参数必须以标准表示法指向IPv6地址的文本表示形式。
			//@pAddrBuf:      指向缓冲区的指针，用于存储IP地址的数字二进制表示形式。 IP地址以网络字节顺序返回。
			//                当Family参数为AF_INET时，此缓冲区应足够大以容纳IN_ADDR结构。
			//	              当Family参数为AF_INET6时，此缓冲区应足够大以容纳IN6_ADDR结构。
			//返回值：如果没有错误发生，InetPton函数返回值1，pAddrBuf参数指向的缓冲区包含网络字节顺序的二进制数字IP地址。
			//        如果pAddrBuf参数指向的字符串不是有效的IPv4点分十进制字符串或有效的IPv6地址字符串，则InetPton函数返回值0。 否则，返回值-1，并且可以通过调用来检索特定的错误代码

			IN6_ADDR in6_addr;
			ZeroMemory(&in6_addr, sizeof(in6_addr));
			int nRet = InetPtonW(AF_INET6, ip, &in6_addr);
			if (nRet == 0)
			{
				LOG_SYSERR << L"ip地址字符串拼写错误：" << ip;
			}
			else if(nRet != 1)
			{
				LOG_SYSERR << L"从ip地址字符串到二进制出错";
			}

			addr6->sin6_addr = in6_addr;
			addr6->sin6_port = htons(port);
		}
		void CACGSocket::ToIp(WCHAR * ip, size_t length, const SOCKADDR * sockaddr)
		{
			if (sockaddr->sa_family == AF_INET)
			{
				ToIp(ip, length, sockaddr_in_cast(sockaddr));
			}
			else if(sockaddr->sa_family == AF_INET6)
			{
				ToIp(ip, length, sockaddr_in6_cast(sockaddr));
			}
		}
		void CACGSocket::ToIp(WCHAR * ip, size_t length, const SOCKADDR_IN * sockaddr)
		{
			InetNtopW(AF_INET, &sockaddr->sin_addr, ip, length);
		}
		void CACGSocket::ToIp(WCHAR * ip, size_t length, const SOCKADDR_IN6 * sockaddr)
		{
			InetNtopW(AF_INET6, &sockaddr->sin6_addr, ip, length);
		}

		void CACGSocket::ToIpPort(WCHAR * ip, size_t length, const SOCKADDR * sockaddr)
		{
			if (sockaddr->sa_family == AF_INET)
			{
				ToIpPort(ip, length, sockaddr_in_cast(sockaddr));
			}
			else if (sockaddr->sa_family == AF_INET6)
			{
				ToIpPort(ip, length, sockaddr_in6_cast(sockaddr));
			}
		}
		void CACGSocket::ToIpPort(WCHAR * ip, size_t length, const SOCKADDR_IN * sockaddr)
		{
			ToIp(ip, length, sockaddr);
			size_t ipLength = 0;
			StringCchLengthW(ip, length-1, &ipLength);
			StringCchPrintfW(ip + ipLength, length - 1 - ipLength, L":%u", sockaddr->sin_port);
		}
		void CACGSocket::ToIpPort(WCHAR * ip, size_t length, const SOCKADDR_IN6 * sockaddr)
		{
			ToIp(ip, length, sockaddr);
			size_t ipLength = 0;
			StringCchLengthW(ip, length - 1, &ipLength);
			StringCchPrintfW(ip + ipLength, length - 1 - ipLength, L":%u", sockaddr->sin6_port);
		}
		const SOCKADDR_IN * CACGSocket::sockaddr_in_cast(const SOCKADDR * sockaddr)
		{
			return static_cast<const SOCKADDR_IN*>(acg::base::implicit_cast<const void*>(sockaddr));
		}

		const SOCKADDR_IN6* CACGSocket::sockaddr_in6_cast(const SOCKADDR * sockaddr)
		{
			return static_cast<const SOCKADDR_IN6*>(acg::base::implicit_cast<const void*>(sockaddr));
		}
		const SOCKADDR * CACGSocket::sockaddr_cast(const SOCKADDR_IN6 * sockaddr_in6)
		{
			return static_cast<const SOCKADDR*>(acg::base::implicit_cast<const void*>(sockaddr_in6));
		}
    }
}
