#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include "D3D.h"

using namespace DirectX;

struct RMObject
{
	XMFLOAT3 Position;
	int Type;
	XMFLOAT3 Rotation;
	float Scale;
};

struct RMObjectCollectionData
{
	int Count;
	XMFLOAT3 __padding;

	RMObject Objects[256];
};

struct LightData
{
	XMFLOAT3 LightDirection;
	float LightIntensity;
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
};

struct CameraData
{
	XMFLOAT4X4 ViewProjectionMatrix;
	float FOV;
	XMFLOAT3 __padding;
};

struct PerRenderingData
{
	float Time;
	float Width;
	float Height;
	float _padding;

	CameraData CameraData;

	LightData LightData;

	RMObjectCollectionData ObjectsData;
};

class PerRenderingDataContainer
{

protected:
	int createDX11Buffer(ID3D11Device* pD3DDevice);

public:

	int init(D3D* d3d);
	void deinit();

	void clear();
	void bind(ID3D11DeviceContext* pD3DDeviceCOntext);

	void SetTime(float time);
	void SetResolution(float width, float height);
	void SetCameraData(float fov, XMFLOAT4X4* view, XMFLOAT4X4* projection);
	void SetLightData(LightData data);

private:

	PerRenderingData _data = {};
	ID3D11Buffer* _buffer = nullptr;
};