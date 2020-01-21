#include "stdafx.h"
#include "Material.h"
#include "App.h"


namespace acg
{

	namespace Render
	{

		void ColorMaterial::BuildConstantBuffers()
		{
			m_constantBuffer = std::make_unique<Render::UploadBuffer<Render::ObjectConstants>>(g_App->GetDevice().Get(), 1, true);

			D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc{};
			constantBufferViewDesc.SizeInBytes = m_constantBuffer->GetElementByteSize();                      //元素的对齐大小
			constantBufferViewDesc.BufferLocation = m_constantBuffer->Resource()->GetGPUVirtualAddress();     //第i个元素的起始地址，这里i为0。

			//将 常量缓冲区资源 和 CBV描述符 绑定。
			g_App->GetDevice()->CreateConstantBufferView(&constantBufferViewDesc, m_CBVSRVUAVDHeap->GetCPUDescriptorHandleForHeapStart());
		}

		void ColorMaterial::BuildRootSignature()
		{
			//根参数数组。根参数类型可以是根常量，根描述符或者描述符表。
			CD3DX12_ROOT_PARAMETER1 RP[1];

			//描述符表（也就是描述符的数组）
			CD3DX12_DESCRIPTOR_RANGE1 cbvTable;
			cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,    //描述符表类型
				1,                                            //表中描述符的数量
				0);                                           //将这段描述符表绑定至此基准着色器寄存器

			RP[0].InitAsDescriptorTable(1, &cbvTable);        //描述符表的数组

			//根签名描述符
			D3D12_ROOT_SIGNATURE_DESC1 desc{};
			desc.NumParameters = 1;            //根参数的数量
			desc.pParameters = RP;             //根参数数组。
			desc.NumStaticSamplers = 0;
			desc.pStaticSamplers = nullptr;
			desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			D3D12_VERSIONED_ROOT_SIGNATURE_DESC DRSDesc{};
			DRSDesc.Desc_1_1 = desc;
			DRSDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;

			Microsoft::WRL::ComPtr<ID3DBlob> SerializedRootSig = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob = nullptr;

			HRESULT hr = S_OK;

			hr = D3D12SerializeVersionedRootSignature(&DRSDesc, SerializedRootSig.GetAddressOf(), ErrorBlob.GetAddressOf());

			if (ErrorBlob != nullptr)
			{
				OutputDebugStringA((LPCSTR)ErrorBlob->GetBufferPointer());
			}

			//创建仅含一个槽位（该槽位指向一个仅由一个常量缓冲区组成的描述符表）的根签名。
			g_App->GetDevice()->CreateRootSignature(0, SerializedRootSig->GetBufferPointer(), SerializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));
		}

		void ColorMaterial::CreateCBVSRVUAVDescriptorHeaps(UINT Num)
		{
			D3D12_DESCRIPTOR_HEAP_DESC CBV{};
			CBV.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			CBV.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			CBV.NumDescriptors = Num;
			CBV.NodeMask = 0;

			ThrowIfFailed(g_App->GetDevice()->CreateDescriptorHeap(&CBV, IID_PPV_ARGS(&m_CBVSRVUAVDHeap)));
		}

		void ColorMaterial::BuildShaders()
		{
			m_mvsByteCode = Render::ComplieShader(L"Shaders\\Color.hlsl", nullptr, "VS", "vs_5_1");
			m_mpsByteCode = Render::ComplieShader(L"Shaders\\Color.hlsl", nullptr, "PS", "ps_5_1");
		}

	}
}

