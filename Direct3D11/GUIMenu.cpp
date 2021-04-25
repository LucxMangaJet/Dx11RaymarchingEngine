#include "GUIMenu.h"
#include "imgui.h"

void GUIMenu::Draw(const AppInfo& AppInfo)
{
	if (!_isShown) return;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Info"))
			{
				NotifyCallback(GUIMenuEventType::Window_Info);
			}

			if (ImGui::MenuItem("Console"))
			{
				NotifyCallback(GUIMenuEventType::Window_Console);
			}

			if (ImGui::MenuItem("Demo Window"))
			{
				NotifyCallback(GUIMenuEventType::Window_Demo);
			}

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Quit"))
		{
			NotifyCallback(GUIMenuEventType::Quit);
		}

		ImGui::EndMainMenuBar();
	}
}
