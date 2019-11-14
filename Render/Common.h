#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

#include <wrl.h>

#include <d3d12.h>
#include <dxgi1_4.h>
#include "d3dx12.h"
#include <DirectXMath.h>


class Common
{
public:
	Common();
	~Common();
};

namespace acg
{

}

#ifndef ThrowIfFailed
#define ThrowIfFailed(x)              \
{                                     \
	HRESULT Z__h = x;                 \
	if (FAILED(Z__h))                 \
	{                                 \
         exit(1);                     \
	}                                 \
}                                     
#endif // !ThrowIfFailed


#ifndef ReleaseCom
#define ReleaseCom(x) {if(x) x->Release();x=0;}
#endif // !ReleaseCom


