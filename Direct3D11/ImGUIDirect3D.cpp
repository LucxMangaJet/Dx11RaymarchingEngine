#include "ImGUIDirect3D.h"

InitResult ImGUIDirect3D::Initialize(const AppInfo& appInfo)
{
    /*
	// Setup backend capabilities flags
	ImGuiIO& io = ImGui::GetIO();
	io.BackendRendererName = "imgui_impl_dx11";
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.

	// Get factory from device
	IDXGIDevice* pDXGIDevice = NULL;
	IDXGIAdapter* pDXGIAdapter = NULL;
	IDXGIFactory* pFactory = NULL;

	if (device->QueryInterface(IID_PPV_ARGS(&pDXGIDevice)) == S_OK)
		if (pDXGIDevice->GetParent(IID_PPV_ARGS(&pDXGIAdapter)) == S_OK)
			if (pDXGIAdapter->GetParent(IID_PPV_ARGS(&pFactory)) == S_OK)
			{
				g_pd3dDevice = device;
				g_pd3dDeviceContext = device_context;
				g_pFactory = pFactory;
			}
	if (pDXGIDevice) pDXGIDevice->Release();
	if (pDXGIAdapter) pDXGIAdapter->Release();
	g_pd3dDevice->AddRef();
	g_pd3dDeviceContext->AddRef();

	return true;
    */

    return InitResult();
}

void ImGUIDirect3D::Update(const AppInfo& appInfo)
{
	/*
	 if (!g_pFontSampler)
		ImGui_ImplDX11_CreateDeviceObjects();
	
	*/
}

void ImGUIDirect3D::Render(const AppInfo& appInfo, ImDrawData* draw_data)
{
}

void ImGUIDirect3D::DeInitialize()
{
	/*
	  ImGui_ImplDX11_InvalidateDeviceObjects();
	if (g_pFactory) { g_pFactory->Release(); g_pFactory = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	*/
}
