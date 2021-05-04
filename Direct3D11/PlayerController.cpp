#include "PlayerController.h"
#include "Camera.h"
#include "Engine.h"
#include "AppInfo.h"
#include "Time.h"
#include "Physics.h"
#include "ExtraMath.h"
#include <cmath>
#include "loguru.hpp"

InitResult PlayerController::Init(const AppInfo& appInfo, Camera* camera)
{
	_oldMousePos = {};
	_oldMousePos.x = appInfo.Width * 0.5f;
	_oldMousePos.y = appInfo.Height * 0.5f;

	_camera = camera;

	_movementSpeed = 10;

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

		V3 eular = _camera->GetWorldRotation();
		eular.y += (_mouseSensitivity * diff.x) / appInfo.Width;
		eular.x += (_mouseSensitivity * diff.y) / appInfo.Height;

		_camera->SetEulerAngles(eular);
		_oldMousePos = mousePos;
	}

	V3 position = _camera->GetWorldPosition();
	V3 forward = _camera->GetForwardVector();
	V3 right = _camera->GetRightVector();

	forward.y = 0;
	forward = Normalize(forward);
	right.y = 0;
	right = Normalize(right);

	float deltaTime = appInfo.Time->GetDeltaTime();


	if (GetKeyState(KEY_W) & KEY_PRESSED)
	{
		position = position + (forward * (deltaTime * _movementSpeed));
	}
	if (GetKeyState(KEY_S) & KEY_PRESSED)
	{
		position = position + (forward * (-deltaTime * _movementSpeed));
	}
	if (GetKeyState(KEY_A) & KEY_PRESSED)
	{
		position = position + (right * (-deltaTime * _movementSpeed));
	}
	if (GetKeyState(KEY_D) & KEY_PRESSED)
	{
		position = position + (right * (deltaTime * _movementSpeed));
	}

	float collisionValue[9];
	Physics* physics = appInfo.Engine->GetPhysics();
	V3 center = _camera->GetWorldPosition() + V3(0, -1, 0);

	for (int i = 0; i < 9; i++)
	{
		collisionValue[i] = physics->GetDistance(_collisionID + i);
		V3 vector = center - _collisionPoints[i];
		float force = max(0, -collisionValue[i]);
		position = position + force * vector;
	}

	//fall
	if (collisionValue[0] < 0)
	{
		_vy = 0;
		if (GetKeyState(KEY_SPACE) & KEY_PRESSED)
		{
			_vy -= 20;
		}
	}
	else
	{
		_vy += 9.81f * deltaTime;
		position = position + (V3(0, -1, 0) * _vy * deltaTime);
	}


	_camera->SetPosition(position);

	_camera->Update();
}

void PlayerController::OnPrePhysics(const AppInfo& appInfo)
{
	Physics* physics = appInfo.Engine->GetPhysics();

	V3 position = _camera->GetWorldPosition();
	V3 point = position + V3(0, -1.5f, 0);
	_collisionPoints[0] = point;

	for (int i = 0; i < _CollisionPointCount - 1; i++)
	{
		float r = ((float)i / _CollisionPointCount - 1) * 2 * PI;
		float x = sin(r);
		float z = cos(r);
		V3 point = position + V3(x, -1, z);
		_collisionPoints[i + 1] = point;
	}

	for (int i = 0; i < _CollisionPointCount; i++)
	{
		physics->SetPoint(_collisionPoints[i]);
	}
}
