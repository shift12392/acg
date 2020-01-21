#include "stdafx.h"
#include "Shape.h"
#include <DirectXColors.h>
#include <array>

#include "App.h"

using namespace DirectX;

void acg::Render::BoxShape::BuildBoxGeometry()
{
	std::array<VertexPC, 8> vertices =
	{
		VertexPC({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
		VertexPC({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
		VertexPC({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
		VertexPC({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
		VertexPC({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
		VertexPC({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
		VertexPC({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
		VertexPC({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
	};

	std::array<std::uint16_t, 36> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	PerVertexByteSize = sizeof(VertexPC);
	AllVertexByteSize = vertices.size() * PerVertexByteSize;
	AllIndexByteSize = indices.size() * sizeof(std::uint16_t);

	ThrowIfFailed( D3DCreateBlob(AllVertexByteSize, &VertexBufferCPU) );
	CopyMemory(VertexBufferCPU->GetBufferPointer(), vertices.data(), AllVertexByteSize);

	ThrowIfFailed(D3DCreateBlob(AllIndexByteSize, &IndexBufferCPU));
	CopyMemory(IndexBufferCPU->GetBufferPointer(), indices.data(), AllIndexByteSize);

	VertexBufferGPU = Render::CreateDefaultBuffer(g_App->GetDevice(), g_App->GetCommandList4(), VertexBufferCPU->GetBufferPointer(), AllVertexByteSize, VertexBufferUploader);
	IndexBufferGPU = Render::CreateDefaultBuffer(g_App->GetDevice(), g_App->GetCommandList4(), IndexBufferCPU->GetBufferPointer(), AllIndexByteSize, IndexBufferUploader);

	SubmeshGeometry submesh;
	submesh.IndexCount = AllIndexByteSize;
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	m_submeshGeometrys.push_back(submesh);
}

void acg::Render::BoxShape::BuildMaterial()
{
	std::shared_ptr<Material> material = std::make_shared<ColorMaterial>(DirectX::XMFLOAT4(1, 1, 1, 1));
	m_pMaterials.push_back(material);
}
