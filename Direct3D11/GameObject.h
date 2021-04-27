#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"
using namespace DirectX;

class GameObject
{
public:

	int Init(ID3D11Device* pD3DDevice, Mesh* mesh, Material* material);
	void update(float deltaTime);
	void render(ID3D11DeviceContext* pD3DDeviceContext);
	void SetPosition(float x, float y, float z);
	V3 GetPosition();

	M4X4* getWorldMatrix() { return &_worldMatrix; }

private:

	V3 _position;
	V3 _euler;

	Mesh* _mesh = nullptr;
	Material* _material = nullptr;
	M4X4 _worldMatrix = {};
};
