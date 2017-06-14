// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "../targetver.h"

#include "acg_switch.h"


// TODO:  在此处引用程序需要的其他头文件

//如果不是按Windows 2008环境编译，立刻报错
#if _WIN32_WINNT < 0x0600 || WINVER < 0x0600
#error  这组服务器类的目标平台为Windows 2008 Server 或者 Windows vista 及以上平台，而当前编译目标平台的版本制定的太低，请检查 _WIN32_WINNT 和 WINVER 的宏定义
#endif


//UNICODE Windows支持
#include <tchar.h>
#include <windows.h>
#include <locale.h>
//使用系统安全字符串函数支持
#include <strsafe.h>
//使用ATL的字符集转换支持
#include <atlconv.h>



// TODO:  在此处引用程序需要的其他头文件
#include "acg_def.h"
