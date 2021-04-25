#include "GUIPLayerController.h"
#include "imgui.h"
#include "PlayerController.h"
#include "AppInfo.h"
#include "Engine.h"

void GUIPLayerController::Init(const AppInfo& appInfo)
{
	_playerController = appInfo.Engine->GetPlayerController();
}

void GUIPLayerController::Draw(const AppInfo& appInfo)
{
	ImGui::Separator();
	ImGui::DragFloat("Movement Speed", &_playerController->_movementSpeed, 1.0f, 1, 10);
	ImGui::DragFloat("Mouse Sensitivity", &_playerController->_mouseSensitivity, 1.0f, 1, 10);
	ImGui::Separator();
}
