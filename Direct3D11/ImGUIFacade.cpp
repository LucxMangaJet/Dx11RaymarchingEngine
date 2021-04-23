#include "ImGUIFacade.h"

InitResult ImGUIFacade::Initialize(const AppInfo& appInfo)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	InitResult result;

	result = _win32Helper.Initialize(appInfo);
	if (result.Failed)return result;

	result = _direct3DHelper.Initialize(appInfo);
	if (result.Failed) return result;

	// Load Fonts - If no fonts are loaded, dear imgui will use the default font.
	//For now lets use default

	return InitResult::Success();
}

void ImGUIFacade::Update(const AppInfo& appInfo)
{
	//// Start the Dear ImGui frame
	_direct3DHelper.Update(appInfo);
	_win32Helper.Update(appInfo);
	ImGui::NewFrame();

	// Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void ImGUIFacade::Render(const AppInfo& appInfo)
{
	ImGui::Render();
	
	//Do i need?? Or do i render stuff combined with the engine?!
	//g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	//g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);

	_direct3DHelper.Render(appInfo, ImGui::GetDrawData());
}

void ImGUIFacade::DeInitialize()
{
	_win32Helper.DeInitialize();
	_direct3DHelper.DeInitialize();
	ImGui::DestroyContext();
}
