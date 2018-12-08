

//acg开关宏定义头文件, 通过修改这个文件中的一些宏开关定义可以定制acg的特性

#pragma once

//关闭4995警告
#pragma warning( disable : 4995 )
// 从 Windows 头中排除极少使用的资料
#define VC_EXTRALEAN


//在windows.h中排除单独引用的APIs,比如WinSock的API
/*
Winsock2.h头文件内部包含来自Windows.h头文件的核心元素，因此Winsock应用程序中通常没有"#include <Windows.h>"。
如果需要"#include <Windows.h>"，则应在之前添加#define WIN32_LEAN_AND_MEAN宏。
由于历史原因，Windows.h标头默认包含Windows套接字1.1的Winsock.h头文件。
Winsock.h头文件中的声明将与Windows Sockets 2.0所需的Winsock2.h头文件中的声明冲突。 
WIN32_LEAN_AND_MEAN宏可防止Windows.h标头包含Winsock.h。
https://docs.microsoft.com/zh-cn/windows/desktop/winsock/creating-a-basic-winsock-application
*/
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


//只适用于简体中文版
#pragma setlocale("chinese-simplified")