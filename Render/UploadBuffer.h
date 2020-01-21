#pragma once

#include "Common.h"

namespace acg
{
	namespace Render
	{
		//��GPU��ȡ��Դ����ʱ��ǧ��Ҫ����Դ����д������

		template<typename T>
		class UploadBuffer
		{
		private:
			Microsoft::WRL::ComPtr<ID3D12Resource> m_uploadBuffer;   //������������Դ
			BYTE* m_MapData = nullptr;
			UINT m_elementByteSize = 0;
			bool m_bIsConstantBuffer = false;

		public:
			UploadBuffer(ID3D12Device5 *pDevice5, UINT elementCount, bool bIsConstantBuffer)
				:m_bIsConstantBuffer(bIsConstantBuffer)
			{
				m_elementByteSize = sizeof(T);

				//�����������Ĵ�С��ΪӲ����С����ռ䣨256�ֽڣ�����������
				if (bIsConstantBuffer)
					m_elementByteSize = CalcConstantBufferByteSize(m_elementByteSize);

				//������������Դͨ����CPUÿ֡����һ�Σ�����Ҫ�ѳ�����������Դ��ŵ�GPU�ϴ����С�
				ThrowIfFailed(pDevice5->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(elementCount*m_elementByteSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&m_uploadBuffer)));

				//�õ���������Դ���ݵ�ָ��
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
				//��Data��ϵͳ�ڴ濽����������������
				memcpy(&m_MapData[ElementIndex*m_elementByteSize], &Data, sizeof(T));
			}

		};
	}
}



