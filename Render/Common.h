#pragma once

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ�ļ����ų�����ʹ�õ�����
// Windows ͷ�ļ�
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


