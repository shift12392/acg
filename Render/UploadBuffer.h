#pragma once

#include "Common.h"

namespace acg
{
	namespace Render
	{
		//在GPU读取资源数据时，千万不要向资源进行写操作。

		template<typename T>
		class UploadBuffer
		{
		private:
			Microsoft::WRL::ComPtr<ID3D12Resource> m_uploadBuffer;   //常量缓冲区资源
			BYTE* m_MapData = nullptr;
			UINT m_elementByteSize = 0;
			bool m_bIsConstantBuffer = false;

		public:
			UploadBuffer(ID3D12Device5 *pDevice5, UINT elementCount, bool bIsConstantBuffer)
				:m_bIsConstantBuffer(bIsConstantBuffer)
			{
				m_elementByteSize = sizeof(T);

				//常量缓冲区的大小必为硬件最小分配空间（256字节）的整数倍。
				if (bIsConstantBuffer)
					m_elementByteSize = CalcConstantBufferByteSize(m_elementByteSize);

				//常量缓冲区资源通常由CPU每帧更新一次，所以要把常量缓冲区资源存放到GPU上传堆中。
				ThrowIfFailed(pDevice5->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(elementCount*m_elementByteSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&m_uploadBuffer)));

				//得到欲更新资源数据的指针
				ThrowIfFailed(m_uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MapData)));

			}
			~UploadBuffer() 
			{
				if (m_uploadBuffer)
					m_uploadBuffer->Unmap(0, nullptr);

				m_MapData = nullptr;
			}

			UploadBuffer(const UploadBuffer& rhs) = delete;
			UploadBuffer& operator= (const UploadBuffer& rhs) = delete;

			ID3D12Resource* Resource() const
			{
				return m_uploadBuffer.Get();
			}

			UINT GetElementByteSize() const { return m_elementByteSize; }

			void CopyData(UINT ElementIndex, const T& Data)
			{
				//把Data从系统内存拷贝到常量缓冲区中
				memcpy(&m_MapData[ElementIndex*m_elementByteSize], &Data, sizeof(T));
			}

		};
	}
}



