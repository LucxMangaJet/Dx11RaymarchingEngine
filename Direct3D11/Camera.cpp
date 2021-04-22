#include "Camera.h"
#include <DirectXMath.h>

using namespace DirectX;



int Camera::init(INT screenWidth, INT screenHeight, float fov, XMFLOAT3 position, XMFLOAT3 eular)
{
	_worldPosition = position;
	_eularAngles = eular;
	this->fov = fov;

	Update();

	return 0;
}

void Camera::deInit()
{
}

DirectX::XMFLOAT3 Camera::getForwardVector()
{
	XMMATRIX matrix = XMLoadFloat4x4(&_viewMatrix); 
	XMFLOAT3 vector;
	XMStoreFloat3(&vector, matrix.r[2]);
	return vector;
}

XMFLOAT3 Camera::getRightVector()
{
	XMMATRIX matrix = XMLoadFloat4x4(&_viewMatrix);  
	XMFLOAT3 vector;
	XMStoreFloat3(&vector, matrix.r[0]);
	return vector;
}

void Camera::SetPosition(XMFLOAT3 position)
{
	_worldPosition = position;
}

void Camera::SetEulerAngles(XMFLOAT3 euler)
{
	_eularAngles = euler;
}

void Camera::Update()
{
	
	XMVECTOR pos = XMLoadFloat3(&_worldPosition);
	XMMATRIX transl = XMMatrixTranslationFromVector(pos);

	XMVECTOR eular = XMLoadFloat3(&_eularAngles);
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(eular);

	XMMATRIX matrx = rotation * transl;

	DirectX::XMStoreFloat4x4(&_viewMatrix, matrx);
}
