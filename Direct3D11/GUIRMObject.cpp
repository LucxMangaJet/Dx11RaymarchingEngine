#include "GUIRMObject.h"
#include "imgui.h";
#include "AppInfo.h"
#include "Engine.h"

void GUIObjects::Draw(const AppInfo& appInfo)
{
	std::vector<RMObject*>const  objects = *appInfo.Engine->GetAllObjects();

	_isUnfolded = ImGui::CollapsingHeader("Objects");
	if (!_isUnfolded) return;

	if (ImGui::Button("AddObject"))
	{
		appInfo.Engine->CreateObject(RMObjectType::Sphere);
		LOG_F(INFO, "Created Object through UI");
	}

	for (int i = 0; i < objects.size(); i++)
	{
		RMObject* object = objects[i];
		auto nr = std::to_string(i);

		ImGui::TextWrapped((nr + ": " + object->Name).c_str());
		ImGui::DragFloat3(("Position " + nr).c_str(), (float*)&object->Position, 0.1);
		ImGui::DragFloat3(("Rotation " + nr).c_str(), (float*)&object->EularAngles, 0.1);
		ImGui::DragFloat3(("Scale " + nr).c_str(), (float*)&object->Scale, 0.1);
		ImGui::Spacing();
		ImGui::DragFloat3(("Repetition " + nr).c_str(), (float*)&object->Repetition, 0.1);
		ImGui::DragInt(("Type " + nr).c_str(), (int*)&object->Type);
	}
}
