#include "stdafx.h"
#include "Common.h"


Common::Common()
{
}


Common::~Common()
{
}

Microsoft::WRL::ComPtr<ID3D12Resource> acg::Render::CreateDefaultBuffer(
	Microsoft::WRL::ComPtr<ID3D12Device5> &Device,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> &CommandList4,
	const void *InitData,
	UINT byteSize,
	Microsoft::WRL::ComPtr<ID3D12Resource> &UploadBuffer)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> DefaultBuffer;

	//创建默认buffer资源和默认堆
	ThrowIfFailed(Device.Get()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(DefaultBuffer.GetAddressOf())));

	//创建上传buffer资源和上传堆
	ThrowIfFailed(Device.Get()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(UploadBuffer.GetAddressOf())
	));

}
