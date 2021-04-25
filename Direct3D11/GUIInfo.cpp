#include "GUIInfo.h"
#include "AppInfo.h"
#include "Engine.h"

void GUIInfo::Init(const AppInfo& appInfo)
{
	guiPLayerController.Init(appInfo);
}

void GUIInfo::Draw(const AppInfo& info)
{
	if (!_isShown) return;

	ImGui::SetNextWindowPos(_position, ImGuiCond_Once);
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);

	ImGui::Begin("Info");                   
	
	ImGui::Text("%.1f FPS (%.1f ms)",ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

	guiPLayerController.Draw(info);

	guiObjects.Draw(info);

	ImGui::End();

}
