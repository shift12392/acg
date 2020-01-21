#pragma once

#include <DirectXMath.h>
#include "Common.h"
#include "UploadBuffer.h"
#include "MathHelper.h"


namespace acg
{

	namespace Render
	{

		struct ObjectConstants
		{
			DirectX::XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
		};


		class Material
		{
		public:
			DirectX::XMFLOAT4 m_BaseColor;
			Microsoft::WRL::ComPtr<ID3DBlob> m_mvsByteCode = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob> m_mpsByteCode = nullptr;

			Material(DirectX::XMFLOAT4 BaseColor)
				:m_BaseColor(BaseColor)
			{

			}

		};

		class ColorMaterial : public Material
		{
		private:

			std::unique_ptr<UploadBuffer<ObjectConstants>> m_constantBuffer;    //常量缓冲区资源
			Microsoft::WRL::ComPtr<ID3D12RootSignature>  m_RootSignature;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_CBVSRVUAVDHeap;

		public:
			ColorMaterial(DirectX::XMFLOAT4 BaseColor)
				:Material(BaseColor)
			{
				CreateCBVSRVUAVDescriptorHeaps(1);
				BuildConstantBuffers();
				BuildRootSignature();
				BuildShaders();
			}

			void CreateCBVSRVUAVDescriptorHeaps(UINT Num);
			void BuildConstantBuffers();
			void BuildRootSignature();
			void BuildShaders();
		};
	}
}

