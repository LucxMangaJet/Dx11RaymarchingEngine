#include "Direct3D.h"
#include "AppInfo.h"

InitResult Direct3D::Initialize(const AppInfo& appInfo)
{
	// 1. create device, device context & swap chain
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 1;
	desc.BufferDesc.Width = appInfo.Width;
	desc.BufferDesc.Height = appInfo.Height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.OutputWindow = appInfo.MainWindow;
	desc.Windowed = appInfo.IsWindowed;

	D3D_FEATURE_LEVEL supportedLevels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D_FEATURE_LEVEL choosenLevel;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr, // graphic adapter (here primary one)
		D3D_DRIVER_TYPE_HARDWARE, // driver type, hardware or software rendering?
		nullptr, // software renderer if driver type is software
		0, // optional flags
		supportedLevels, 6, // supported direct 3d versions (versions & array size)
		D3D11_SDK_VERSION, // api version the application was build with
		&desc, &_pD3DSwapChain, &_pD3DDevice,
		&choosenLevel, // optional parameter for chosen feature level
		&_pD3DDeviceContext
	);
	if (FAILED(hr)) return InitResult::Failure(20, "DX11: Failed to create swap chain.");

	// 2. create render target view
	InitResult result = CreateRenderTargetView();
	if (result.Failed) return result;
	// 3. create depth stencil view
	result = CreateDepthStencilView(appInfo);
	if (result.Failed) return result;

	// 4. create rasterizer state
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;

	hr = _pD3DDevice->CreateRasterizerState(&rsDesc, &_pRasterizerState);
	if (FAILED(hr)) return InitResult::Failure(29, "DX11: Failed to create rasterizer state.");

	// 5. create viewport
	_viewPort.Width = appInfo.Width;
	_viewPort.Height = appInfo.Height;
	_viewPort.TopLeftX = 0.0f;
	_viewPort.TopLeftY = 0.0f;
	_viewPort.MinDepth = 0.0f;
	_viewPort.MaxDepth = 1.0f;

	// 6. prepare the rendering pipeline
	_pD3DDeviceContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);
	_pD3DDeviceContext->RSSetState(_pRasterizerState);
	_pD3DDeviceContext->RSSetViewports(1, &_viewPort);

	return InitResult::Success();
}

InitResult Direct3D::CreateRenderTargetView()
{

	ID3D11Texture2D* pBackBuffer = nullptr;
	HRESULT hr = _pD3DSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr)) return InitResult::Failure(22, "DX11: Failed to retrieved back buffer from swap chain.");

	hr = _pD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	if (FAILED(hr)) return InitResult::Failure(24, "DX11: Failed to create render target view.");

	SafeRelease<ID3D11Texture2D>(pBackBuffer);
	return InitResult::Success();
}

InitResult Direct3D::CreateDepthStencilView(const AppInfo& appInfo)
{
	ID3D11Texture2D* pDepthStencilTexture = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilTextureDesc = {};
	depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTextureDesc.Width = appInfo.Width;
	depthStencilTextureDesc.Height = appInfo.Height;
	depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilTextureDesc.ArraySize = 1;
	depthStencilTextureDesc.SampleDesc.Count = 1;

	HRESULT hr = _pD3DDevice->CreateTexture2D(&depthStencilTextureDesc, nullptr, &pDepthStencilTexture);
	if (FAILED(hr)) return InitResult::Failure(26, "DX11: Failed to create depth stencil texture.");

	hr = _pD3DDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &_pDepthStencilView);
	if (FAILED(hr)) return InitResult::Failure(28, "DX11: Failed to create depth stencil view.");

	SafeRelease<ID3D11Texture2D>(pDepthStencilTexture);

	return InitResult::Success();
}



void Direct3D::BeginScene(float red, float green, float blue)
{
	// clear back buffer with solid color
	const float color[] = { red, green, blue, 1.0f };
	_pD3DDeviceContext->ClearRenderTargetView(_pRenderTargetView, color);
	_pD3DDeviceContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0xffffff);
}

void Direct3D::EndScene()
{
	// swap back with front buffer
	_pD3DSwapChain->Present(0, 0);
}

void Direct3D::DeInitialize()
{
	SafeRelease<ID3D11RasterizerState>(_pRasterizerState);
	SafeRelease<ID3D11DepthStencilView>(_pDepthStencilView);
	SafeRelease<ID3D11RenderTargetView>(_pRenderTargetView);
	SafeRelease<ID3D11Device>(_pD3DDevice);
	SafeRelease<ID3D11DeviceContext>(_pD3DDeviceContext);
	SafeRelease<IDXGISwapChain>(_pD3DSwapChain);
}


void Direct3D::OnResize(const AppInfo& appInfo)
{

	SafeRelease(_pRenderTargetView);
	SafeRelease(_pDepthStencilView);
	_pD3DDeviceContext->OMSetRenderTargets(0, 0, 0);

	_pD3DSwapChain->ResizeBuffers(0, appInfo.Width, appInfo.Height, DXGI_FORMAT_UNKNOWN, 0);

	CreateRenderTargetView();
	CreateDepthStencilView(appInfo);

	_viewPort.Width = appInfo.Width;
	_viewPort.Height = appInfo.Height;
	_viewPort.TopLeftX = 0.0f;
	_viewPort.TopLeftY = 0.0f;
	_viewPort.MinDepth = 0.0f;
	_viewPort.MaxDepth = 1.0f;

	_pD3DDeviceContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);
	_pD3DDeviceContext->RSSetViewports(1, &_viewPort);
}
