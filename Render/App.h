#pragma once

#include <memory>

#include "Common.h"
#include "UploadBuffer.h"
#include "MathHelper.h"
#include "Shape.h"

#include <functional>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"D3D12.lib")
#pragma comment(lib,"dxgi.lib")

namespace acg
{
	class App
	{
	public:
		static const UINT SwapChainBufferCount = 2;
	private:
		HINSTANCE m_Instance = nullptr;
		HWND m_hWnd = nullptr;

		Microsoft::WRL::ComPtr<IDXGIFactory4>   m_DXGIFactory4;
		Microsoft::WRL::ComPtr<ID3D12Device5>   m_Device5;
		Microsoft::WRL::ComPtr<ID3D12Fence1>    m_Fence1;

		Microsoft::WRL::ComPtr<ID3D12CommandQueue>     m_CommandQueue;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4>      m_CommandList4;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RTVHeap;                    //渲染目标视图描述符堆
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DSVHeap;                    

		Microsoft::WRL::ComPtr<IDXGISwapChain> m_DXGISwapChain;                               //交换链
		Microsoft::WRL::ComPtr<ID3D12Resource> m_SwapChainBuffer[SwapChainBufferCount];       
		Microsoft::WRL::ComPtr<ID3D12Resource> m_DepthStencilBuffer;

		DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		D3D12_VIEWPORT m_ScreenViewport;
		D3D12_RECT m_ScissorRect;             //剪裁矩形

		UINT m_RTVDescriptorSize = 0;
		UINT m_DSVDescriptorSize = 0;
		UINT m_CBVSRVUAVDescriptorSize = 0;

		bool m_bUse4xMsaa = false;
		UINT m_4xMsaaQuality = 0;

		UINT m_ClientWidth = 1600;
		UINT m_ClientHeight = 1024;

		UINT m_CurrentBackBuffer = 0;

		UINT64 m_CurrentFence = 0;              //围栏

		std::vector<std::shared_ptr<Render::Shape>> m_shapes;

	public:
		//事件
		typedef std::function<void(WPARAM btnState, int x, int y)> MouseEvent;

		MouseEvent m_MouseDownEvent;
		MouseEvent m_MouseMoveEvent;
		MouseEvent m_MouseUpEvent;

	public:
		App(HINSTANCE hInstance);
		~App();

		virtual bool Initialize(int nCmdShow);
		virtual int Run();

		LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		Microsoft::WRL::ComPtr<ID3D12Device5>   GetDevice() const {return m_Device5;}
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> GetCommandList4() const { return m_CommandList4; }


		void AddShape(std::shared_ptr<Render::Shape> NewShape)
		{
			m_shapes.push_back(NewShape);
		}

	protected:
		virtual void OnResize();
		virtual void Update();
		virtual void Draw();

		bool InitWindow(int nCmdShow);
		bool InitD3D();

		void FlushCommandQueue();

		void CreateCommandObjects();

		//创建交换链
		void CreateSwapChain();      
		void CreateRTVAndDSVDscriptorHeaps();

		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

#if defined(DEBUG) || defined(_DEBUG)
		void LogAdapters();
		void LogAdapterOutputs(IDXGIAdapter *Adapter);
		//每个显示设备都有一系列它所支持的的显示模式。
		void LogOutputDisplayModes(IDXGIOutput *Output, DXGI_FORMAT Format);
#endif
	};


	extern 	App* g_App;

}



