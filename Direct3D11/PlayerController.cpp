#include "PlayerController.h"
#include <DirectXMath.h>
#include "Camera.h"
#include "AppInfo.h"
#include "Time.h"

InitResult PlayerController::Init(const AppInfo& appInfo, Camera* camera)
{
	_oldMousePos = {};
	_oldMousePos.x = appInfo.Width * 0.5f;
	_oldMousePos.y = appInfo.Height * 0.5f;

	_camera = camera;

	_movementSpeed = 10; //Hardcoded

	return InitResult();
}

void PlayerController::Update(const AppInfo& appInfo)
{

	POINT mousePos;
	if (GetCursorPos(&mousePos))
	{
		POINT diff;
		diff.x = mousePos.x - _oldMousePos.x;
		diff.y = mousePos.y - _oldMousePos.y;

		XMFLOAT3 eular = _camera->GetWorldRotation();
		eular.y += (_mouseSensitivity * diff.x) / appInfo.Width;
		eular.x += (_mouseSensitivity * diff.y) / appInfo.Height;

		_camera->SetEulerAngles(eular);
		_oldMousePos = mousePos;
	}

	XMFLOAT3 f_position = _camera->GetWorldPosition();
	XMFLOAT3 f_forward = _camera->GetForwardVector();
	XMFLOAT3 f_right = _camera->GetRightVector();

	XMVECTOR position = XMLoadFloat3(&f_position);
	XMVECTOR forward = XMLoadFloat3(&f_forward);
	XMVECTOR right = XMLoadFloat3(&f_right);

	float deltaTime = appInfo.Time->GetDeltaTime();

	if (GetKeyState(KEY_W) & KEY_PRESSED)
	{
		position = XMVectorAdd(position, XMVectorScale(forward, deltaTime * _movementSpeed));
	}
	if (GetKeyState(KEY_S) & KEY_PRESSED)
	{
		position = XMVectorAdd(position, XMVectorScale(forward, -deltaTime * _movementSpeed));
	}
	if (GetKeyState(KEY_A) & KEY_PRESSED)
	{
		position = XMVectorAdd(position, XMVectorScale(right, -deltaTime * _movementSpeed));
	}
	if (GetKeyState(KEY_D) & KEY_PRESSED)
	{
		position = XMVectorAdd(position, XMVectorScale(right, deltaTime * _movementSpeed));
	}

	XMStoreFloat3(&f_position, position);
	_camera->SetPosition(f_position);

	_camera->Update();
}