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
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> &CommandList,
	const void *InitData,
	UINT byteSize,
	Microsoft::WRL::ComPtr<ID3D12Resource> &UploadBuffer)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> DefaultBuffer;

	//����Ĭ��buffer��Դ��Ĭ�϶�
	ThrowIfFailed(Device.Get()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(DefaultBuffer.GetAddressOf())));

	//Ϊ�˽�cpu�˵��ڴ��е����ݸ��Ƶ�Ĭ�ϻ����������ǻ���Ҫ����һ�������н�λ�õ��ϴ�buffer��Դ���ϴ���
	ThrowIfFailed(Device.Get()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(UploadBuffer.GetAddressOf())
	));

	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = InitData;
	subResourceData.RowPitch = byteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	CommandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST));

	UpdateSubresources<1>(CommandList.Get(), DefaultBuffer.Get(), UploadBuffer.Get(), 0, 0, 1, &subResourceData);

	CommandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ));

	return DefaultBuffer;

}
