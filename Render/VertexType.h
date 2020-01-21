#pragma once



#include "Common.h"

namespace acg
{
	struct VertexPC
	{
		VertexPC(DirectX::XMFLOAT3 InPos, DirectX::XMFLOAT4 InColor)
			:Pos(InPos), Color(InColor)
		{

		}

		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
	};

	//D3D12_INPUT_ELEMENT_DESC VertexPCInputElementDescs[2] =
	//{
	//	{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,offsetof(VertexPC,Pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	//	{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,offsetof(VertexPC,Color), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
	//};

	struct VertexPNTT
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Normal;
		DirectX::XMFLOAT2 Tex0;
		DirectX::XMFLOAT2 Tex1;
	};


	//D3D12_INPUT_ELEMENT_DESC VertexPCNTTInputElementDescs[4]
	//{
	//	{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,offsetof(VertexPNTT,Pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	//	{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,offsetof(VertexPNTT,Normal), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	//	{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,offsetof(VertexPNTT,Tex0), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	//	{"TEXCOORD",1,DXGI_FORMAT_R32G32_FLOAT,0,offsetof(VertexPNTT,Tex1), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
	//};
}




