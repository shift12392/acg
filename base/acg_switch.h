

//acg开关宏定义头文件, 通过修改这个文件中的一些宏开关定义可以定制acg的特性

#pragma once

//关闭4995警告
#pragma warning( disable : 4995 )
// 从 Windows 头中排除极少使用的资料
#define VC_EXTRALEAN	
//在windows.h中排除单独引用的APIs,比如WinSock的API
#define WIN32_LEAN_AND_MEAN	
//只适用于简体中文版
#pragma setlocale("chinese-simplified")