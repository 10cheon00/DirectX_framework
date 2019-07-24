#include "Graphics.h"



bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	if (!InitializeDirectX(hwnd, width, height))
		return false;

	if (!InistializeShaders())
		return false;
	return true;
}

void Graphics::RenderFrame()
{
	float bgcolor[] = { 1.0f,0.0f,1.0f,1.0f };
	this->deviceContext->ClearRenderTargetView(
		this->renderTargetView.Get(), bgcolor);
	this->swapChain->Present(1/*vsync option*/, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1) {
		ErrorLogger::Log("No IDXGI Adapters. found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd,sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr;

	hr= D3D11CreateDeviceAndSwapChain(
		adapters[0].pAdapter,	//IDXGIAdapter
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,	//For Software driver type
		NULL,	//flags for runtime layers
		NULL,	//Feature levels array
		0,	//Number of feature levels in array
		D3D11_SDK_VERSION,
		&scd,
		this->swapChain.GetAddressOf(),
		this->device.GetAddressOf(),
		NULL,	//Support feature level
		this->deviceContext.GetAddressOf()
	);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create device and swapchain.");
		return false;
	}
	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create Render target view.");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), NULL);

	return true;
}

bool Graphics::InistializeShaders()
{
	std::wstring shaderfolder;
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE) {
	#ifdef _DEBUG
		#ifdef _WIN64
				shaderfolder = L"..\\x64\\Debug\\";
		#else
				shaderfolder = L"..\\Debug\\";
		#endif
	#else	//ReleaseMode
		#ifdef _WIN64
				shaderfolder = L"..\\x64\\Release\\";
		#else
				shaderfolder = L"..\\Release\\";
		#endif
#endif
	}


	if (!vertexShader.Initialize(this->device, shaderfolder + L"vertexshader.cso"))
		return false;
	
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layout);

	HRESULT hr = this->device->CreateInputLayout(
		layout,
		numElements,
		this->vertexShader.GetBuffer()->GetBufferPointer(),
		this->vertexShader.GetBuffer()->GetBufferSize(),
		this->inputLayout.GetAddressOf());

	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to create input layout.");
		return false;
	}

	return true;
}
