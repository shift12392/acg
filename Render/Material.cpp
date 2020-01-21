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
			constantBufferViewDesc.SizeInBytes = m_constantBuffer->GetElementByteSize();                      //Ԫ�صĶ����С
			constantBufferViewDesc.BufferLocation = m_constantBuffer->Resource()->GetGPUVirtualAddress();     //��i��Ԫ�ص���ʼ��ַ������iΪ0��

			//�� ������������Դ �� CBV������ �󶨡�
			g_App->GetDevice()->CreateConstantBufferView(&constantBufferViewDesc, m_CBVSRVUAVDHeap->GetCPUDescriptorHandleForHeapStart());
		}

		void ColorMaterial::BuildRootSignature()
		{
			//���������顣���������Ϳ����Ǹ���������������������������
			CD3DX12_ROOT_PARAMETER1 RP[1];

			//��������Ҳ���������������飩
			CD3DX12_DESCRIPTOR_RANGE1 cbvTable;
			cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,    //������������
				1,                                            //����������������
				0);                                           //�����������������˻�׼��ɫ���Ĵ���

			RP[0].InitAsDescriptorTable(1, &cbvTable);        //�������������

			//��ǩ��������
			D3D12_ROOT_SIGNATURE_DESC1 desc{};
			desc.NumParameters = 1;            //������������
			desc.pParameters = RP;             //���������顣
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

			//��������һ����λ���ò�λָ��һ������һ��������������ɵ����������ĸ�ǩ����
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

