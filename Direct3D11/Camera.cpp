#include "Camera.h"
#include <DirectXMath.h>

using namespace DirectX;



int Camera::init(INT screenWidth, INT screenHeight, float fov, XMFLOAT3 position, XMFLOAT3 forward, XMFLOAT3 up)
{
	_worldPosition = position;
	_forward = forward;
	_up = up;
	this->fov = fov;

	Update();
	return 0;
}

void Camera::deInit()
{
}

void Camera::SetPosition(XMFLOAT3 position)
{
	_worldPosition = position;
}

void Camera::Update()
{
	XMVECTOR pos = DirectX::XMLoadFloat3(&_worldPosition);
	XMVECTOR fwd = DirectX::XMLoadFloat3(&_forward);
	XMVECTOR xmup = DirectX::XMLoadFloat3(&_up);
	// view matrix
	XMMATRIX viewMatrix = XMMatrixLookToLH(pos, fwd, xmup);
	DirectX::XMStoreFloat4x4(&_viewMatrix, viewMatrix);
}
