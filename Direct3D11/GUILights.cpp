#include "GUILights.h"
#include "imgui.h"
#include "Lights.h"
#include "AppInfo.h"
#include "Engine.h"

void GUILights::Draw(const AppInfo& appInfo)
{
	LightData* lights = appInfo.Engine->GetLights()->GetLightData();

	_isUnfolded = ImGui::CollapsingHeader("Lights");
	if (!_isUnfolded) return;

	ImGui::Separator();

	ImGui::DragFloat3("Directional Light", (float*)&lights->LightDirection, 0.1);
	ImGui::SliderFloat("Light Intensity", &lights->LightIntensity, 0, 1);

	ImGui::ColorPicker4("Ambient Color", (float*)&lights->AmbientColor);
	ImGui::ColorPicker4("Diffuse Color", (float*)&lights->DiffuseColor);
	ImGui::Separator();
}
