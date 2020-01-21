#pragma once


#include <vector>
#include <DirectXMath.h>
#include <DirectXCollision.h>


#include "Common.h"
#include "VertexType.h"
#include "Material.h"


namespace acg
{
	namespace Render
	{

		struct SubmeshGeometry
		{
			UINT IndexCount = 0;
			UINT StartIndexLocation = 0;
			INT BaseVertexLocation = 0;

			DirectX::BoundingBox Bounds;
		};

		class Shape
		{
		public:
			Microsoft::WRL::ComPtr<ID3DBlob>  VertexBufferCPU = nullptr;
			Microsoft::WRL::ComPtr<ID3DBlob>  IndexBufferCPU = nullptr;

			Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
			Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;

			Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
			Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;

			UINT AllVertexByteSize = 0;            //顶点缓冲区总的字节大小
			UINT PerVertexByteSize = 0;            //每个顶点的大小
			DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;   //索引格式
			UINT AllIndexByteSize = 0;             //索引缓冲区总的字节大小

			std::vector<D3D12_INPUT_ELEMENT_DESC>  InputDescs;
			std::vector< std::shared_ptr<Material> >              m_pMaterials;
			std::vector<SubmeshGeometry>           m_submeshGeometrys;

		public:
			virtual ~Shape() {}


			D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
			{
				D3D12_VERTEX_BUFFER_VIEW vbv;
				vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();              //顶点缓冲区资源的GPU虚拟地址
				vbv.SizeInBytes = AllVertexByteSize;
				vbv.StrideInBytes = PerVertexByteSize;

				return vbv;
			}

			D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
			{
				D3D12_INDEX_BUFFER_VIEW ibv;
				ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
				ibv.Format = IndexFormat;
				ibv.SizeInBytes = AllIndexByteSize;

				return ibv;
			}

		};

		class BoxShape : public Shape
		{
		private:

		public:

			BoxShape()
			{
				InputDescs.push_back( D3D12_INPUT_ELEMENT_DESC{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,offsetof(VertexPC,Pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 });
				InputDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,offsetof(VertexPC,Color), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 });

				BuildBoxGeometry();
				BuildMaterial();
			}

			void BuildBoxGeometry();
			void BuildMaterial();
		};
	}

}

