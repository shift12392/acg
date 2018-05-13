#pragma once

//#include <Winsock.h> //确定使用Winsock1时 还原这个 而注释下一行
#include <WinSock2.h>
#include <MSWSock.h>


//#pragma comment( lib, "WSOCK32.lib" ) //确定使用Winsock1时 还原这个 而注释下一行
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")
