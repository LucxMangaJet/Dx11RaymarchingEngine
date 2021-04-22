#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	int init(INT screenWidth, INT screenHeight, float fov, XMFLOAT3 position, XMFLOAT3 eular);
	void deInit();

	XMFLOAT3 getWorldPosition() { return _worldPosition; }
	XMFLOAT3 getWorldRotation() { return _eularAngles; }
	XMFLOAT3 getForwardVector();
	XMFLOAT3 getRightVector();

	XMFLOAT4X4* getViewMatrix() { return &_viewMatrix; }
	

	float getFOV() { return fov; }

	void SetPosition(XMFLOAT3 position);
	void SetEulerAngles(XMFLOAT3 euler);
	void Update();

private:
	XMFLOAT3 _worldPosition;
	XMFLOAT3 _eularAngles;

	XMFLOAT4X4 _viewMatrix = {};
	float fov;
};
