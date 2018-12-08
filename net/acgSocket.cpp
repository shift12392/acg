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
			    �ں������listen����֮ǰ����δ���ӵ��׽�������Ҫ����bind������ 
				��ͨ�����ڰ󶨵��������ӣ������������ӣ����ݱ������׽��֡� 
				bind����Ҳ�����ڰ󶨵�ԭʼ�׽��֣��׽�����ͨ������socket���������ģ����Ͳ�������ΪSOCK_RAW���� 
				��connect��ConnectEx��WSAConnect��WSAConnectByList��WSAConnectByName��������֮ǰ���󶨺���Ҳ������δ���ӵ��׽�����ʹ�á�

				��ͨ������socket���������׽���ʱ���������������ռ䣨��ַ�壩�У���û��Ϊ��������ơ�
				ʹ��bind����ͨ��Ϊδ�������׽��ַ��䱾�������������׽��ֵı��ع�����

				ʹ��Internet��ַ��ʱ����������������ɣ�
					��ַ�塣
					������ַ��
					��ʶӦ�ó���Ķ˿ںš�

				��Windows�׽���2�У�name���������ϸ����Ϊָ��sockaddr�ṹ��ָ�롣
				�������ַ�ʽת��ΪWindows Sockets 1.1�����ԡ�
				Service providers���Խ�����Ϊָ��namelen��С���ڴ���ָ�롣
				�˿��е�ǰ2���ֽڣ���Ӧ��sockaddr�ṹ��sa_family��Ա��sockaddr_in�ṹ��sin_family��Ա��sockaddr_in6�ṹ��sin6_family��Ա������������ڴ����׽��ֵĵ�ַ�塣 
				���򣬻ᷢ������WSAEFAULT��

				���Ӧ�ó��򲻹��ķ������ĸ����ص�ַ����ΪIPv4���ص�ַָ������ֵINADDR_ANY��������name������sa_data��Ա��ΪIPv6���ص�ַָ������ֵin6addr_any�� ������ײ�����ṩ��ʹ���κ��ʵ��������ַ�������ڴ��ڶ����������������ж������ӿں͵�ַ��������������¼�Ӧ�ó����̡�

				����TCP / IP������˿�ָ��Ϊ�㣬������ṩ�ߴӶ�̬�ͻ��˶˿ڷ�ΧΪӦ�ó������Ψһ�˿ڡ� ��Windows Vista�����߰汾�У���̬�ͻ��˶˿ڷ�Χ�ǽ���49152��65535֮���ֵ�����Ƕ�Windows Server 2003������汾�ĸ��ģ����ж�̬�ͻ��˶˿ڷ�Χ�ǽ���1025��5000֮���ֵ���ͻ��˶�̬�����ֵ ����ͨ��������ע�����������ֵ�����Ķ˿ڷ�Χ��HKLM\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters
				MaxUserPortע���ֵ�������ڶ�̬�ͻ��˶˿ڷ�Χ�����ֵ��ֵ�� �����������������������ʹ��������Ч��

				��Windows Vista�����߰汾�У�����ʹ��netsh����鿴�͸��Ķ�̬�ͻ��˶˿ڷ�Χ�� ����ΪUDP��TCP�Լ�IPv4��IPv6���ò�ͬ�Ķ�̬�ͻ��˶˿ڷ�Χ�� �йظ�����Ϣ����μ�֪ʶ������929851��

				�ڵ���bind֮��Ӧ�ó������ʹ��getsockname���õ��ѷ�����׽��ֵĵ�ַ�Ͷ˿ڡ� ���Internet��ַ����INADDR_ANY��in6addr_any������the socket connected֮ǰ��getsockname��һ�����ṩ��ַ����Ϊ��������Ƕ������ģ��򼸸���ַ������Ч�� ������ͻ���Ӧ�ó���󶨵��˿�0������ض��˿ںţ���Ϊ���������ڱ��ؼ������ʹ�øö˿ںŵ���һ���׽��ֳ�ͻ��Σ�ա�

				ע�⣺��ʹ�ô���SO_EXCLUSIVEADDRUSE��SO_REUSEADDR�׽���ѡ���bindʱ��������ִ��bind֮ǰ�����׽���ѡ����ܲ����κ�Ӱ�졣 �й���ϸ��Ϣ�������SO_EXCLUSIVEADDRUSE��ʹ��SO_REUSEADDR��SO_EXCLUSIVEADDRUSE��

			*/
			if (0 != bind(handle, addr, sizeof(struct sockaddr)))
			{
				LOG_SYSFATAL << L"��ʧ��";
			}
		}
		void CACGSocket::FromIpPort(const WCHAR * ip, USHORT port, sockaddr_in * addr)
		{
			IN_ADDR in_addr;
			ZeroMemory(&in_addr, sizeof(in_addr));
			int nRet = InetPtonW(AF_INET, ip, &in_addr);

			if (nRet == 0)
			{
				LOG_SYSERR << L"ip��ַ�ַ���ƴд����" << ip;
			}
			else if (nRet != 1)
			{
				LOG_SYSERR << L"��ip��ַ�ַ����������Ƴ���";
			}

			addr->sin_addr.S_un.S_addr = in_addr.S_un.S_addr;
			addr->sin_port = htons(port);
		}
		void CACGSocket::FromIp6Port(const WCHAR * ip, USHORT port, sockaddr_in6 * addr6)
		{
			//InetPton��������׼�ı���ʾ��ʽ��IPv4��IPv6 Internet�����ַת��Ϊ�����ֶ�������ʽ�� �˺�����ANSI�汾Ϊinet_pton��
			//@Family����ַ��Ŀ���ֵ��Ws2def.hͷ�ļ��ж��塣 ��ע�⣬Ws2def.hͷ�ļ��Զ�������Winsock2.h�У���Ӧֱ��ʹ�á� ��ע�⣬AF_��ַ���PF_Э���峣����ֵ����ͬ�ģ����磬AF_INET��PF_INET������˿���ʹ�ó�����
			//         Ŀǰ��֧��AF_INET��AF_INET6
			//@pszAddrString :ָ����NULL��β���ַ�����ָ�룬���ַ�������Ҫת��Ϊ���ֶ�������ʽ��IP��ַ���ı���ʾ��ʽ��
			//                ��Family����ΪAF_INETʱ��pszAddrString���������Ա�׼���ʮ���Ʊ�ʾ��ָ��IPv4��ַ���ı���ʾ��ʽ��
			//	              ��Family����ΪAF_INET6ʱ��pszAddrString���������Ա�׼��ʾ��ָ��IPv6��ַ���ı���ʾ��ʽ��
			//@pAddrBuf:      ָ�򻺳�����ָ�룬���ڴ洢IP��ַ�����ֶ����Ʊ�ʾ��ʽ�� IP��ַ�������ֽ�˳�򷵻ء�
			//                ��Family����ΪAF_INETʱ���˻�����Ӧ�㹻��������IN_ADDR�ṹ��
			//	              ��Family����ΪAF_INET6ʱ���˻�����Ӧ�㹻��������IN6_ADDR�ṹ��
			//����ֵ�����û�д�������InetPton��������ֵ1��pAddrBuf����ָ��Ļ��������������ֽ�˳��Ķ���������IP��ַ��
			//        ���pAddrBuf����ָ����ַ���������Ч��IPv4���ʮ�����ַ�������Ч��IPv6��ַ�ַ�������InetPton��������ֵ0�� ���򣬷���ֵ-1�����ҿ���ͨ�������������ض��Ĵ������

			IN6_ADDR in6_addr;
			ZeroMemory(&in6_addr, sizeof(in6_addr));
			int nRet = InetPtonW(AF_INET6, ip, &in6_addr);
			if (nRet == 0)
			{
				LOG_SYSERR << L"ip��ַ�ַ���ƴд����" << ip;
			}
			else if(nRet != 1)
			{
				LOG_SYSERR << L"��ip��ַ�ַ����������Ƴ���";
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
