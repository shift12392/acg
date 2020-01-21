#include "stdafx.h"
#include "Common.h"

Common::Common()
{
}


Common::~Common()
{
}

namespace acg
{
	namespace Render
	{

		//------------------------------------------------------------------------------------------------
		// All arrays must be populated (e.g. by calling GetCopyableFootprints)
		inline UINT64 UpdateSubresources(
			_In_ ID3D12GraphicsCommandList4* pCmdList,
			_In_ ID3D12Resource* pDestinationResource,
			_In_ ID3D12Resource* pIntermediate,
			_In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
			_In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources,
			UINT64 RequiredSize,
			_In_reads_(NumSubresources) const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts,
			_In_reads_(NumSubresources) const UINT* pNumRows,
			_In_reads_(NumSubresources) const UINT64* pRowSizesInBytes,
			_In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData)
		{
			// Minor validation
			auto IntermediateDesc = pIntermediate->GetDesc();
			auto DestinationDesc = pDestinationResource->GetDesc();
			if (IntermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER ||
				IntermediateDesc.Width < RequiredSize + pLayouts[0].Offset ||
				RequiredSize > SIZE_T(-1) ||
				(DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER &&
				(FirstSubresource != 0 || NumSubresources != 1)))
			{
				return 0;
			}

			BYTE* pData;
			HRESULT hr = pIntermediate->Map(0, nullptr, reinterpret_cast<void**>(&pData));
			if (FAILED(hr))
			{
				return 0;
			}

			for (UINT i = 0; i < NumSubresources; ++i)
			{
				if (pRowSizesInBytes[i] > SIZE_T(-1)) return 0;
				D3D12_MEMCPY_DEST DestData = { pData + pLayouts[i].Offset, pLayouts[i].Footprint.RowPitch, SIZE_T(pLayouts[i].Footprint.RowPitch) * SIZE_T(pNumRows[i]) };
				MemcpySubresource(&DestData, &pSrcData[i], static_cast<SIZE_T>(pRowSizesInBytes[i]), pNumRows[i], pLayouts[i].Footprint.Depth);
			}
			pIntermediate->Unmap(0, nullptr);

			if (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
			{
				pCmdList->CopyBufferRegion(
					pDestinationResource, 0, pIntermediate, pLayouts[0].Offset, pLayouts[0].Footprint.Width);
			}
			else
			{
				for (UINT i = 0; i < NumSubresources; ++i)
				{
					CD3DX12_TEXTURE_COPY_LOCATION Dst(pDestinationResource, i + FirstSubresource);
					CD3DX12_TEXTURE_COPY_LOCATION Src(pIntermediate, pLayouts[i]);
					pCmdList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
				}
			}
			return RequiredSize;
		}

		//------------------------------------------------------------------------------------------------
		// Heap-allocating UpdateSubresources implementation
		inline UINT64 UpdateSubresources(
			_In_ ID3D12GraphicsCommandList4* pCmdList,
			_In_ ID3D12Resource* pDestinationResource,
			_In_ ID3D12Resource* pIntermediate,
			UINT64 IntermediateOffset,
			_In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
			_In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources,
			_In_reads_(NumSubresources) D3D12_SUBRESOURCE_DATA* pSrcData)
		{
			UINT64 RequiredSize = 0;
			UINT64 MemToAlloc = static_cast<UINT64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64)) * NumSubresources;
			if (MemToAlloc > SIZE_MAX)
			{
				return 0;
			}
			void* pMem = HeapAlloc(GetProcessHeap(), 0, static_cast<SIZE_T>(MemToAlloc));
			if (pMem == nullptr)
			{
				return 0;
			}
			auto pLayouts = reinterpret_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(pMem);
			UINT64* pRowSizesInBytes = reinterpret_cast<UINT64*>(pLayouts + NumSubresources);
			UINT* pNumRows = reinterpret_cast<UINT*>(pRowSizesInBytes + NumSubresources);

			auto Desc = pDestinationResource->GetDesc();
			ID3D12Device5* pDevice = nullptr;
			ThrowIfFailed(pDestinationResource->GetDevice(__uuidof(ID3D12Device5), reinterpret_cast<void**>(&pDevice)));
			pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, pLayouts, pNumRows, pRowSizesInBytes, &RequiredSize);
			pDevice->Release();

			UINT64 Result = UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, pLayouts, pNumRows, pRowSizesInBytes, pSrcData);
			HeapFree(GetProcessHeap(), 0, pMem);
			return Result;
		}

		//------------------------------------------------------------------------------------------------
		// Stack-allocating UpdateSubresources implementation
		template <UINT MaxSubresources>
		inline UINT64 UpdateSubresources(
			_In_ ID3D12GraphicsCommandList4* pCmdList,
			_In_ ID3D12Resource* pDestinationResource,
			_In_ ID3D12Resource* pIntermediate,
			UINT64 IntermediateOffset,
			_In_range_(0, MaxSubresources) UINT FirstSubresource,
			_In_range_(1, MaxSubresources - FirstSubresource) UINT NumSubresources,
			_In_reads_(NumSubresources) D3D12_SUBRESOURCE_DATA* pSrcData)
		{
			UINT64 RequiredSize = 0;
			D3D12_PLACED_SUBRESOURCE_FOOTPRINT Layouts[MaxSubresources];
			UINT NumRows[MaxSubresources];
			UINT64 RowSizesInBytes[MaxSubresources];

			auto Desc = pDestinationResource->GetDesc();
			ID3D12Device5* pDevice = nullptr;
			ThrowIfFailed(pDestinationResource->GetDevice(__uuidof(ID3D12Device5), reinterpret_cast<void**>(&pDevice)));
			pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, Layouts, NumRows, RowSizesInBytes, &RequiredSize);
			pDevice->Release();

			return UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource, NumSubresources, RequiredSize, Layouts, NumRows, RowSizesInBytes, pSrcData);
		}
	}
}


Microsoft::WRL::ComPtr<ID3D12Resource> acg::Render::CreateDefaultBuffer(
	Microsoft::WRL::ComPtr<ID3D12Device5> Device,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> CommandList4,
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

	//为了将cpu端的内存中的数据复制到默认缓存区，我们还需要创建一个处于中介位置的上传buffer资源和上传堆
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

	CommandList4->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST));

	UpdateSubresources<1>(CommandList4.Get(), DefaultBuffer.Get(), UploadBuffer.Get(), 0, 0, 1, &subResourceData);

	CommandList4->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ));

	return DefaultBuffer;

}

Microsoft::WRL::ComPtr<ID3DBlob> acg::Render::ComplieShader(LPCWSTR filename,
	const D3D_SHADER_MACRO* defines,
	LPCSTR entrypoint,
	LPCSTR target)
{

	UINT compileFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // DEBUG 

	HRESULT hRet = S_OK;

	Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errors;
	hRet = D3DCompileFromFile(filename, defines, nullptr, entrypoint, target, compileFlags, 0, byteCode.GetAddressOf(), errors.GetAddressOf());

	if (errors != nullptr)
	{
		OutputDebugStringA((char*)errors->GetBufferPointer());
	}

	ThrowIfFailed(hRet);

	return byteCode;
}
