#include "GUIPlayerController.h"
#include "imgui.h"
#include "PlayerController.h"
#include "AppInfo.h"
#include "Engine.h"

void GUIPlayerController::Init(const AppInfo& appInfo)
{
	_playerController = appInfo.Engine->GetPlayerController();
	_camera = appInfo.Engine->GetActiveCamera();
}

void GUIPlayerController::Draw(const AppInfo& appInfo)
{
	ImGui::Separator();
	ImGui::DragFloat("Movement Speed", &_playerController->_movementSpeed, 1.0f, 1, 10);
	ImGui::DragFloat("Mouse Sensitivity", &_playerController->_mouseSensitivity, 1.0f, 1, 10);
	ImGui::DragFloat3("Camera Position", (float*)_camera->GetWorldPositionPtr(), 0.1);
	ImGui::DragFloat3("Camera Rotation", (float*)_camera->GetWorldRotationPtr(), 0.1);


	ImGui::Separator();
}
