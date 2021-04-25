#include "GUIInfo.h"


void GUIInfo::Draw(const AppInfo& info)
{
	if (!_isShown) return;

	ImGui::SetNextWindowPos(_position, ImGuiCond_Once);
	ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);

	ImGui::Begin("Info");                   
	
	ImGui::Text("%.1f FPS (%.1f ms)",ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

	ImGui::End();

}
