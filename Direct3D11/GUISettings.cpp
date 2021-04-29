#include "GUISettings.h"
#include "AppInfo.h"
#include "Engine.h"
#include "ShaderHandler.h"

void GUISettings::Init(const AppInfo& appInfo)
{
	_guiPLayerController.Init(appInfo);
}

void GUISettings::Draw(const AppInfo& info)
{
	if (!_isShown) return;

	ImGui::SetNextWindowPos(_position, ImGuiCond_Once);
	ImGui::SetNextWindowSize(_size, ImGuiCond_Once);
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);

	ImGui::Begin("Settings");                   
	ImGui::Spacing();
	ImGui::Text("%.1f FPS (%.1f ms)",ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Spacing();

	if (ImGui::Button("Recompile all Shaders (Requires restart)"))
	{
		info.ShaderHander->LoadAllShaders(true);
	}

	_guiPLayerController.Draw(info);

	_lights.Draw(info);

	_guiObjects.Draw(info);

	ImGui::End();

}
