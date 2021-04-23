#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "AppInfo.h"

using namespace DirectX;

class Camera
{
public:
	InitResult Initialize(INT screenWidth, INT screenHeight, float fov, XMFLOAT3 position, XMFLOAT3 eular);
	void DeInitialize();

	XMFLOAT3 GetWorldPosition() { return _worldPosition; }
	XMFLOAT3 GetWorldRotation() { return _eularAngles; }
	XMFLOAT3 GetForwardVector();
	XMFLOAT3 GetRightVector();

	XMFLOAT4X4* GetViewMatrix() { return &_viewMatrix; }
	

	float GetFOV() { return fov; }

	void SetPosition(XMFLOAT3 position);
	void SetEulerAngles(XMFLOAT3 euler);
	void Update();

private:
	XMFLOAT3 _worldPosition;
	XMFLOAT3 _eularAngles;

	XMFLOAT4X4 _viewMatrix = {};
	float fov;
};
