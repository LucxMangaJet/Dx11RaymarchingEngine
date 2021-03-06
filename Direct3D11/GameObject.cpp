#include "GameObject.h"
#include "Vertex.h"

int GameObject::Init(ID3D11Device* pD3DDevice, Mesh* mesh, Material* material)
{
	_material = material;
	_mesh = mesh;
	XMStoreFloat4x4(&_worldMatrix, XMMatrixIdentity());
	return 0;
}

void GameObject::update(float deltaTime)
{
	XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(_euler.x, _euler.y, _euler.z);
	XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	XMStoreFloat4x4(&_worldMatrix, scale * rotation * translation);
}

void GameObject::render(ID3D11DeviceContext* pD3DDeviceContext)
{
	_material->Render(pD3DDeviceContext, &_worldMatrix);
	_mesh->Render(pD3DDeviceContext);
}

void GameObject::SetPosition(float x, float y, float z)
{
	_position = { x, y, z };
}

V3 GameObject::GetPosition()
{
	return _position;
}