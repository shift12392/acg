#pragma once

//#include <Winsock.h> //ȷ��ʹ��Winsock1ʱ ��ԭ��� ��ע����һ��
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

//���Ӧ�ó�������ʹ��IP Helper API������ҪIphlpapi.hͷ�ļ��� ����ҪIphlpapi.hͷ�ļ�ʱ��Winsock2.hͷ�ļ���#include��Ӧ�÷���Iphlpapi.hͷ�ļ���#include��֮ǰ��
//https://docs.microsoft.com/zh-cn/windows/desktop/winsock/sockaddr-2
//#include <iphlpapi.h>         


//#pragma comment( lib, "WSOCK32.lib" ) //ȷ��ʹ��Winsock1ʱ ��ԭ��� ��ע����һ��
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")
