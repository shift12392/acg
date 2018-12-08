#pragma once

//#include <Winsock.h> //确定使用Winsock1时 还原这个 而注释下一行
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

//如果应用程序正在使用IP Helper API，则需要Iphlpapi.h头文件。 当需要Iphlpapi.h头文件时，Winsock2.h头文件的#include行应该放在Iphlpapi.h头文件的#include行之前。
//https://docs.microsoft.com/zh-cn/windows/desktop/winsock/sockaddr-2
//#include <iphlpapi.h>         


//#pragma comment( lib, "WSOCK32.lib" ) //确定使用Winsock1时 还原这个 而注释下一行
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")
