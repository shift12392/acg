#pragma once

#include "Common.h"


namespace acg
{

	class Shape
	{
	public:
		Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;

		UINT AllVertexByteSize = 0;
		UINT PerVertexButeSize = 0;

	public:
		virtual ~Shape() {}


		D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
		{
			D3D12_VERTEX_BUFFER_VIEW vbv;
			vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
			vbv.SizeInBytes = AllVertexByteSize;
			vbv.StrideInBytes = PerVertexButeSize;

			return vbv;
		}



	};
}

