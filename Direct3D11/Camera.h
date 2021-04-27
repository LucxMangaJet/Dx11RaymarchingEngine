#pragma once
#include <d3d11.h>
#include "AppInfo.h"

class Camera
{
public:
	InitResult Initialize(INT screenWidth, INT screenHeight, float fov, V3 position, V3 eular);
	void DeInitialize();

	V3 GetWorldPosition() { return _worldPosition; }
	V3 GetWorldRotation() { return _eularAngles; }

	V3* GetWorldPositionPtr() { return &_worldPosition; }
	V3* GetWorldRotationPtr() { return &_eularAngles; }

	V3 GetForwardVector();
	V3 GetRightVector();

	M4X4* GetViewMatrix() { return &_viewMatrix; }
	

	float GetFOV() { return fov; }

	void SetPosition(V3 position);
	void SetEulerAngles(V3 euler);
	void Update();

private:
	V3 _worldPosition;
	V3 _eularAngles;

	M4X4 _viewMatrix = {};
	float fov;
};
