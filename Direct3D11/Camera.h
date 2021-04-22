#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	int init(INT screenWidth, INT screenHeight, float fov, XMFLOAT3 position, XMFLOAT3 forward, XMFLOAT3 up);
	void deInit();

	XMFLOAT3 getWorldPosition() { return _worldPosition; }
	XMFLOAT4X4* getViewMatrix() { return &_viewMatrix; }
	float getFOV() { return fov; }

	void SetPosition(XMFLOAT3 position);
	void Update();

private:
	XMFLOAT3 _worldPosition;
	XMFLOAT3 _forward;
	XMFLOAT3 _up;

	XMFLOAT4X4 _viewMatrix = {};
	float fov;
};
