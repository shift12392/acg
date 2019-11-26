#include "stdafx.h"
#include "Resource.h"
#include "App.h"

#include <strsafe.h>

#include <vector>
#include <assert.h>

using namespace Microsoft::WRL;

namespace acg
{

#define MAX_LOADSTRING 100
	WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
	WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

	App* g_App = nullptr;

	//
	//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
	//
	//  目标: 处理主窗口的消息。
	//
	//  WM_COMMAND  - 处理应用程序菜单
	//  WM_PAINT    - 绘制主窗口
	//  WM_DESTROY  - 发送退出消息并返回
	//
	//
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return g_App->Proc(hWnd, message, wParam, lParam);
	}

	// “关于”框的消息处理程序。
	INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
		return (INT_PTR)FALSE;
	}



	App::App(HINSTANCE hInstance)
		:m_Instance(hInstance)
	{
	}


	App::~App()
	{
	}

	bool App::Initialize(int nCmdShow)
	{
		if (!InitWindow(nCmdShow))
			return false;

		if (!InitD3D())
			return false;

		return true;
	}

	int App::Run()
	{

		HACCEL hAccelTable = LoadAccelerators(m_Instance, MAKEINTRESOURCE(IDC_RENDER));

		MSG msg;

		// 主消息循环:
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return (int)msg.wParam;
	}

	void App::OnResize()
	{
		//在更改任何资源之前刷新命令队列
		FlushCommandQueue();

		ThrowIfFailed(m_CommandList4->Reset(m_CommandAllocator.Get(), nullptr));

		//释放上一次创建的资源
		for (int i = 0; i < SwapChainBufferCount; i++)
		{
			m_SwapChainBuffer[i].Reset();
		}
		m_DepthStencilBuffer.Reset();

		//更改交换链的大小
		ThrowIfFailed(m_DXGISwapChain->ResizeBuffers(SwapChainBufferCount, m_ClientWidth, m_ClientHeight, m_BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		m_CurrentBackBuffer = 0;

		//创建渲染目标视图，并与渲染目标视图描述符绑定。
		CD3DX12_CPU_DESCRIPTOR_HANDLE RTVDescriptor(m_RTVHeap->GetCPUDescriptorHandleForHeapStart());
		for (int i = 0; i < SwapChainBufferCount; i++)
		{
			ThrowIfFailed(m_DXGISwapChain->GetBuffer(i, IID_PPV_ARGS(m_SwapChainBuffer[i].GetAddressOf())));
			m_Device5->CreateRenderTargetView(
				m_SwapChainBuffer[i].Get(),
				nullptr,                   //如果Resource创建时已经指定了格式，如果这里设置空，则使用Resource的格式。
				RTVDescriptor);            //引用所创建的渲染目标视图的描述符句柄
			RTVDescriptor.Offset(m_RTVDescriptorSize);
		}

		//创建深度/模板Buffer和视图
		// 创建深度/模板Buffer
		D3D12_RESOURCE_DESC DepthStencilDesc{};
		DepthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;           //资源的维度
		DepthStencilDesc.Width = m_ClientWidth;
		DepthStencilDesc.Height = m_ClientHeight;
		DepthStencilDesc.DepthOrArraySize = 1;                 //以纹素为单位，纹理的深度。
		DepthStencilDesc.Alignment = 0;
		DepthStencilDesc.MipLevels = 1;                       //mipmap层级的数量。
		DepthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		DepthStencilDesc.SampleDesc.Count = m_bUse4xMsaa ? 4 : 1;
		DepthStencilDesc.SampleDesc.Quality = m_bUse4xMsaa ? m_4xMsaaQuality - 1 : 0;
		DepthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE ClearValue{};
		ClearValue.Format = m_DepthStencilFormat;
		ClearValue.DepthStencil.Depth = 1.0;
		ClearValue.DepthStencil.Stencil = 0;

		// 这个函数同时也会一个堆，GPU资源都存于堆中，其本质是具有特定属性的GPU显存块。
		ThrowIfFailed(m_Device5->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),       //默认堆，在这个堆里面的资源，唯独GPU可以访问。
			D3D12_HEAP_FLAG_NONE,
			&DepthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,                      //资源的初始状态
			&ClearValue,
			IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf())
		));

		//D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc{};
		//DSVDesc.Flags = D3D12_DSV_FLAG_NONE;
		//DSVDesc.Format = m_DepthStencilFormat;
		//DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		//DSVDesc.Texture2D.MipSlice = 1;

		// 创建深度/模板视图
		m_Device5->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, DepthStencilView());

		// 更改深度/模板资源的状态
		m_CommandList4->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

		//执行命令队列
		ThrowIfFailed(m_CommandList4->Close());       //先“关闭”
		ID3D12CommandList* CommandList[] = { m_CommandList4.Get() };          
		m_CommandQueue->ExecuteCommandLists(1, CommandList);

		//等待执行完成
		FlushCommandQueue();

		m_ScreenViewport.TopLeftX = 0;
		m_ScreenViewport.TopLeftY = 0;
		m_ScreenViewport.Width = static_cast<float>(m_ClientWidth);
		m_ScreenViewport.Height = static_cast<float>(m_ClientHeight);
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;

		m_ScissorRect = { 0, 0, static_cast<LONG>(m_ClientWidth), static_cast<LONG>(m_ClientHeight)};
	}

	LRESULT App::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 分析菜单选择:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(m_Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			m_ClientWidth = LOWORD(lParam);
			m_ClientHeight = HIWORD(lParam);
			if (m_Device5)
			{
				if (wParam == SIZE_RESTORED)
				{
					OnResize();
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					OnResize();
				}
			}
			break;
		//当用户释放调整大小的bars时，发送。
		case WM_EXITSIZEMOVE:
			OnResize();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	bool App::InitWindow(int nCmdShow)
	{
		// 初始化全局字符串
		LoadStringW(m_Instance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		LoadStringW(m_Instance, IDC_RENDER, szWindowClass, MAX_LOADSTRING);

		//注册窗口类
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_Instance;
		wcex.hIcon = LoadIcon(m_Instance, MAKEINTRESOURCE(IDI_RENDER));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RENDER);
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        RegisterClassExW(&wcex);


		m_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_Instance, nullptr);

		if (!m_hWnd)
		{
			return false;
		}

		ShowWindow(m_hWnd, nCmdShow);
		UpdateWindow(m_hWnd);

		return true;
	}

	bool App::InitD3D()
	{
		//启用D3D调试层
#if defined(DEBUG) || defined(_DEBUG)
		{
			ComPtr<ID3D12Debug> debugController;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
			debugController->EnableDebugLayer();
		}
#endif

		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_DXGIFactory4)));

		//创建硬件设备。
		//使用默认的主显示适配器
		ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_Device5)));

		//创建围栏
		ThrowIfFailed(m_Device5->CreateFence(m_CurrentFence, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence1)));

		//得到相应资源描述符的大小
		m_RTVDescriptorSize = m_Device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);         //得到渲染目标视图的大小
		m_DSVDescriptorSize = m_Device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);         //得到深度/模板视图的大小
		m_CBVSRVUAVDescriptorSize = m_Device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);  //得到常量缓冲区视图/着色器资源视图/无序访问视图的大小。

		//检查4X MSAA质量级别支持
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels{};
		QualityLevels.Format = m_BackBufferFormat;
		QualityLevels.SampleCount = 4;
		QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		QualityLevels.NumQualityLevels = 0;
		ThrowIfFailed(m_Device5->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &QualityLevels, sizeof(QualityLevels)));

		m_4xMsaaQuality = QualityLevels.NumQualityLevels;

		assert(m_4xMsaaQuality > 0);

#if defined(DEBUG) || defined(_DEBUG)
		LogAdapters();
#endif

		//创建命令队列和命令列表
		CreateCommandObjects();

		CreateSwapChain();

		CreateRTVAndDSVDscriptorHeaps();

		OnResize();

		return true;
	}

	void App::FlushCommandQueue()
	{
		++m_CurrentFence;

		//向命令队列添加一条设置新围栏的命令
		//由于这条命令要由GPU来处理（即由GPU来修改围栏点），所以在GPU完成处理此Signal()之前的所有命令之前，不会设置新的围栏点。
		ThrowIfFailed(m_CommandQueue->Signal(m_Fence1.Get(), m_CurrentFence));

		//等待命令处理完
		if (m_Fence1->GetCompletedValue() < m_CurrentFence)
		{
			HANDLE hEvent = CreateEventEx(nullptr, NULL, 0, EVENT_ALL_ACCESS);    //无名字，初始状态不触发，自动重置。

			ThrowIfFailed(m_Fence1->SetEventOnCompletion(m_CurrentFence, hEvent));   
			WaitForSingleObject(hEvent, INFINITE);   //一直等待
			CloseHandle(hEvent);     //必须释放句柄
		} 
	}

	void App::CreateCommandObjects()
	{
		D3D12_COMMAND_QUEUE_DESC CommandQueueDesc {};
		CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(m_Device5->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&m_CommandQueue)));

		ThrowIfFailed(m_Device5->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));

		//刚创建的命令列表处于“开启”状态。
		ThrowIfFailed(m_Device5->CreateCommandList(
			0,                                       //该节点掩码指定的是与所建命令列表相关联的物理GPU。 对于仅仅有一个GPU的的系统，设置为0。m_Device5->GetNodeCount()返回物理GPU的数量。
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_CommandAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(m_CommandList4.GetAddressOf())
		));

		m_CommandList4->Close();

	}
	void App::CreateSwapChain()
	{

		m_DXGISwapChain.Reset();

		DXGI_SWAP_CHAIN_DESC SwapChainDesc{};
		SwapChainDesc.BufferDesc.Width = m_ClientWidth;
		SwapChainDesc.BufferDesc.Height = m_ClientHeight;
		SwapChainDesc.BufferDesc.Format = m_BackBufferFormat;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60000;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1000;
		SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
		SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
		SwapChainDesc.SampleDesc.Count = m_bUse4xMsaa ? 4 : 1;
		SwapChainDesc.SampleDesc.Quality = m_bUse4xMsaa ? m_4xMsaaQuality - 1 : 0;   //为什么要减1。
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                 //DXGI_USAGE_RENDER_TARGET_OUTPUT：渲染数据保存后备缓冲区
		SwapChainDesc.BufferCount = SwapChainBufferCount;                                               //交换链中使用的缓冲区数量。这里使用双缓冲
		SwapChainDesc.Windowed = true;                                               //窗口模式
		SwapChainDesc.OutputWindow = m_hWnd;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  //切换到全屏模式时，它将自动选择适用于当前窗口大小的显示模式。如果程序没有指定该标志，当程序切换为全屏模式时，采用当前桌面的显示模式。        

		ThrowIfFailed(m_DXGIFactory4->CreateSwapChain(m_CommandQueue.Get(),&SwapChainDesc, m_DXGISwapChain.GetAddressOf()));
	}

	void App::CreateRTVAndDSVDscriptorHeaps()
	{
		D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc{};
		RTVDescriptorHeapDesc.NumDescriptors = SwapChainBufferCount;
		RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		RTVDescriptorHeapDesc.NodeMask = 0;

		ThrowIfFailed(m_Device5->CreateDescriptorHeap(&RTVDescriptorHeapDesc,IID_PPV_ARGS(m_RTVHeap.GetAddressOf())));

		D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc{};
		DSVDescriptorHeapDesc.NumDescriptors = 1;
		DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DSVDescriptorHeapDesc.NodeMask = 0;

		ThrowIfFailed(m_Device5->CreateDescriptorHeap(&DSVDescriptorHeapDesc, IID_PPV_ARGS(m_DSVHeap.GetAddressOf())));
	}

	D3D12_CPU_DESCRIPTOR_HANDLE App::CurrentBackBufferView() const
	{
		//这里只是复制了ACGD3D12_CPU_DESCRIPTOR_HANDLE的D3D12_CPU_DESCRIPTOR_HANDLE部分。
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_RTVHeap->GetCPUDescriptorHandleForHeapStart(),
			m_CurrentBackBuffer,
			m_RTVDescriptorSize
		);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE App::DepthStencilView() const
	{
		return m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
	}


#if defined(DEBUG) || defined(_DEBUG)

	void App::LogAdapters()
	{
		UINT i = 0;
		IDXGIAdapter *Adapter = nullptr;
		std::vector<IDXGIAdapter*> AdapterList;
		
		while (m_DXGIFactory4->EnumAdapters(i, &Adapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC AdapterDESC;
			Adapter->GetDesc(&AdapterDESC);

			WCHAR str[255];
			StringCchPrintfW(str, 255 * sizeof(WCHAR), L"***Adapter: %s \n", AdapterDESC.Description);

			OutputDebugStringW(str);

			AdapterList.push_back(Adapter);

			++i;
		}

		for (int i = 0; i < AdapterList.size(); i++)
		{
			LogAdapterOutputs(AdapterList[i]);
			ReleaseCom(AdapterList[i]);
		}
	}

	void App::LogAdapterOutputs(IDXGIAdapter * Adapter)
	{
		UINT i = 0;
		IDXGIOutput *Output = nullptr;
		while (Adapter->EnumOutputs(i,&Output) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_OUTPUT_DESC OutputDesc;
			Output->GetDesc(&OutputDesc);

			WCHAR str[255];
			StringCchPrintfW(str, 255 * sizeof(WCHAR), L"***Output: %s \n", OutputDesc.DeviceName);

			OutputDebugStringW(str);

			LogOutputDisplayModes(Output, m_BackBufferFormat);

			ReleaseCom(Output);

			++i;
		}
	}

	void App::LogOutputDisplayModes(IDXGIOutput * Output, DXGI_FORMAT Format)
	{
		UINT flags = 0;
		UINT count = 0;

		//设置为nullptr，得到Format所支持的显示模式的列表数量。
		Output->GetDisplayModeList(Format, flags, &count, nullptr);

		std::vector<DXGI_MODE_DESC> modeList(count);

		//typedef struct DXGI_MODE_DESC
		//{
		//	UINT Width;
		//	UINT Height;
		//	DXGI_RATIONAL RefreshRate;
		//	DXGI_FORMAT Format;
		//	DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;     //逐行扫描 vs 逐列扫描
		//	DXGI_MODE_SCALING Scaling;                     //图像如何相对于屏幕进行拉伸                   
		//} DXGI_MODE_DESC;

		//typedef enum DXGI_MODE_SCALING
		//{
		//	DXGI_MODE_SCALING_UNSPECIFIED = 0,             //未指定
		//	DXGI_MODE_SCALING_CENTERED = 1,                //中心
		//	DXGI_MODE_SCALING_STRETCHED = 2                //stretch 拉伸
		//} DXGI_MODE_SCALING;

		//typedef enum DXGI_MODE_SCANLINE_ORDER
		//{
		//	DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED = 0,             //未指定
		//	DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE = 1,
		//	DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST = 2,
		//	DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST = 3
		//} DXGI_MODE_SCANLINE_ORDER;

		//得到Format所支持的显示模式的列表
		Output->GetDisplayModeList(Format, flags, &count, &modeList[0]);

		for (auto mode : modeList)
		{
			WCHAR str[255];
			StringCchPrintfW(str, 255 * sizeof(WCHAR), L"Width: %d Height: %d Refresh: %d/%d \n", mode.Width, mode.Height, mode.RefreshRate.Numerator, mode.RefreshRate.Denominator);

			OutputDebugStringW(str);
		}
	}

#endif
}
