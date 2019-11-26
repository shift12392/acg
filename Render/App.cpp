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
	WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
	WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

	App* g_App = nullptr;

	//
	//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
	//
	//  Ŀ��: ���������ڵ���Ϣ��
	//
	//  WM_COMMAND  - ����Ӧ�ó���˵�
	//  WM_PAINT    - ����������
	//  WM_DESTROY  - �����˳���Ϣ������
	//
	//
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return g_App->Proc(hWnd, message, wParam, lParam);
	}

	// �����ڡ������Ϣ�������
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

		// ����Ϣѭ��:
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
		//�ڸ����κ���Դ֮ǰˢ���������
		FlushCommandQueue();

		ThrowIfFailed(m_CommandList4->Reset(m_CommandAllocator.Get(), nullptr));

		//�ͷ���һ�δ�������Դ
		for (int i = 0; i < SwapChainBufferCount; i++)
		{
			m_SwapChainBuffer[i].Reset();
		}
		m_DepthStencilBuffer.Reset();

		//���Ľ������Ĵ�С
		ThrowIfFailed(m_DXGISwapChain->ResizeBuffers(SwapChainBufferCount, m_ClientWidth, m_ClientHeight, m_BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		m_CurrentBackBuffer = 0;

		//������ȾĿ����ͼ��������ȾĿ����ͼ�������󶨡�
		CD3DX12_CPU_DESCRIPTOR_HANDLE RTVDescriptor(m_RTVHeap->GetCPUDescriptorHandleForHeapStart());
		for (int i = 0; i < SwapChainBufferCount; i++)
		{
			ThrowIfFailed(m_DXGISwapChain->GetBuffer(i, IID_PPV_ARGS(m_SwapChainBuffer[i].GetAddressOf())));
			m_Device5->CreateRenderTargetView(
				m_SwapChainBuffer[i].Get(),
				nullptr,                   //���Resource����ʱ�Ѿ�ָ���˸�ʽ������������ÿգ���ʹ��Resource�ĸ�ʽ��
				RTVDescriptor);            //��������������ȾĿ����ͼ�����������
			RTVDescriptor.Offset(m_RTVDescriptorSize);
		}

		//�������/ģ��Buffer����ͼ
		// �������/ģ��Buffer
		D3D12_RESOURCE_DESC DepthStencilDesc{};
		DepthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;           //��Դ��ά��
		DepthStencilDesc.Width = m_ClientWidth;
		DepthStencilDesc.Height = m_ClientHeight;
		DepthStencilDesc.DepthOrArraySize = 1;                 //������Ϊ��λ���������ȡ�
		DepthStencilDesc.Alignment = 0;
		DepthStencilDesc.MipLevels = 1;                       //mipmap�㼶��������
		DepthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		DepthStencilDesc.SampleDesc.Count = m_bUse4xMsaa ? 4 : 1;
		DepthStencilDesc.SampleDesc.Quality = m_bUse4xMsaa ? m_4xMsaaQuality - 1 : 0;
		DepthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		DepthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE ClearValue{};
		ClearValue.Format = m_DepthStencilFormat;
		ClearValue.DepthStencil.Depth = 1.0;
		ClearValue.DepthStencil.Stencil = 0;

		// �������ͬʱҲ��һ���ѣ�GPU��Դ�����ڶ��У��䱾���Ǿ����ض����Ե�GPU�Դ�顣
		ThrowIfFailed(m_Device5->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),       //Ĭ�϶ѣ���������������Դ��Ψ��GPU���Է��ʡ�
			D3D12_HEAP_FLAG_NONE,
			&DepthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,                      //��Դ�ĳ�ʼ״̬
			&ClearValue,
			IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf())
		));

		//D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc{};
		//DSVDesc.Flags = D3D12_DSV_FLAG_NONE;
		//DSVDesc.Format = m_DepthStencilFormat;
		//DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		//DSVDesc.Texture2D.MipSlice = 1;

		// �������/ģ����ͼ
		m_Device5->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, DepthStencilView());

		// �������/ģ����Դ��״̬
		m_CommandList4->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

		//ִ���������
		ThrowIfFailed(m_CommandList4->Close());       //�ȡ��رա�
		ID3D12CommandList* CommandList[] = { m_CommandList4.Get() };          
		m_CommandQueue->ExecuteCommandLists(1, CommandList);

		//�ȴ�ִ�����
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
			// �����˵�ѡ��:
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
			// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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
		//���û��ͷŵ�����С��barsʱ�����͡�
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
		// ��ʼ��ȫ���ַ���
		LoadStringW(m_Instance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		LoadStringW(m_Instance, IDC_RENDER, szWindowClass, MAX_LOADSTRING);

		//ע�ᴰ����
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
		//����D3D���Բ�
#if defined(DEBUG) || defined(_DEBUG)
		{
			ComPtr<ID3D12Debug> debugController;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
			debugController->EnableDebugLayer();
		}
#endif

		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_DXGIFactory4)));

		//����Ӳ���豸��
		//ʹ��Ĭ�ϵ�����ʾ������
		ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_Device5)));

		//����Χ��
		ThrowIfFailed(m_Device5->CreateFence(m_CurrentFence, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence1)));

		//�õ���Ӧ��Դ�������Ĵ�С
		m_RTVDescriptorSize = m_Device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);         //�õ���ȾĿ����ͼ�Ĵ�С
		m_DSVDescriptorSize = m_Device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);         //�õ����/ģ����ͼ�Ĵ�С
		m_CBVSRVUAVDescriptorSize = m_Device5->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);  //�õ�������������ͼ/��ɫ����Դ��ͼ/���������ͼ�Ĵ�С��

		//���4X MSAA��������֧��
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

		//����������к������б�
		CreateCommandObjects();

		CreateSwapChain();

		CreateRTVAndDSVDscriptorHeaps();

		OnResize();

		return true;
	}

	void App::FlushCommandQueue()
	{
		++m_CurrentFence;

		//������������һ��������Χ��������
		//������������Ҫ��GPU����������GPU���޸�Χ���㣩��������GPU��ɴ����Signal()֮ǰ����������֮ǰ�����������µ�Χ���㡣
		ThrowIfFailed(m_CommandQueue->Signal(m_Fence1.Get(), m_CurrentFence));

		//�ȴ��������
		if (m_Fence1->GetCompletedValue() < m_CurrentFence)
		{
			HANDLE hEvent = CreateEventEx(nullptr, NULL, 0, EVENT_ALL_ACCESS);    //�����֣���ʼ״̬���������Զ����á�

			ThrowIfFailed(m_Fence1->SetEventOnCompletion(m_CurrentFence, hEvent));   
			WaitForSingleObject(hEvent, INFINITE);   //һֱ�ȴ�
			CloseHandle(hEvent);     //�����ͷž��
		} 
	}

	void App::CreateCommandObjects()
	{
		D3D12_COMMAND_QUEUE_DESC CommandQueueDesc {};
		CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(m_Device5->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&m_CommandQueue)));

		ThrowIfFailed(m_Device5->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));

		//�մ����������б��ڡ�������״̬��
		ThrowIfFailed(m_Device5->CreateCommandList(
			0,                                       //�ýڵ�����ָ�����������������б������������GPU�� ���ڽ�����һ��GPU�ĵ�ϵͳ������Ϊ0��m_Device5->GetNodeCount()��������GPU��������
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
		SwapChainDesc.SampleDesc.Quality = m_bUse4xMsaa ? m_4xMsaaQuality - 1 : 0;   //ΪʲôҪ��1��
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                 //DXGI_USAGE_RENDER_TARGET_OUTPUT����Ⱦ���ݱ���󱸻�����
		SwapChainDesc.BufferCount = SwapChainBufferCount;                                               //��������ʹ�õĻ���������������ʹ��˫����
		SwapChainDesc.Windowed = true;                                               //����ģʽ
		SwapChainDesc.OutputWindow = m_hWnd;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  //�л���ȫ��ģʽʱ�������Զ�ѡ�������ڵ�ǰ���ڴ�С����ʾģʽ���������û��ָ���ñ�־���������л�Ϊȫ��ģʽʱ�����õ�ǰ�������ʾģʽ��        

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
		//����ֻ�Ǹ�����ACGD3D12_CPU_DESCRIPTOR_HANDLE��D3D12_CPU_DESCRIPTOR_HANDLE���֡�
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

		//����Ϊnullptr���õ�Format��֧�ֵ���ʾģʽ���б�������
		Output->GetDisplayModeList(Format, flags, &count, nullptr);

		std::vector<DXGI_MODE_DESC> modeList(count);

		//typedef struct DXGI_MODE_DESC
		//{
		//	UINT Width;
		//	UINT Height;
		//	DXGI_RATIONAL RefreshRate;
		//	DXGI_FORMAT Format;
		//	DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;     //����ɨ�� vs ����ɨ��
		//	DXGI_MODE_SCALING Scaling;                     //ͼ������������Ļ��������                   
		//} DXGI_MODE_DESC;

		//typedef enum DXGI_MODE_SCALING
		//{
		//	DXGI_MODE_SCALING_UNSPECIFIED = 0,             //δָ��
		//	DXGI_MODE_SCALING_CENTERED = 1,                //����
		//	DXGI_MODE_SCALING_STRETCHED = 2                //stretch ����
		//} DXGI_MODE_SCALING;

		//typedef enum DXGI_MODE_SCANLINE_ORDER
		//{
		//	DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED = 0,             //δָ��
		//	DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE = 1,
		//	DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST = 2,
		//	DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST = 3
		//} DXGI_MODE_SCANLINE_ORDER;

		//�õ�Format��֧�ֵ���ʾģʽ���б�
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
