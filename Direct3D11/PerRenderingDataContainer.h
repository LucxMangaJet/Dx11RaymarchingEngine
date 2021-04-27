#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include "D3D.h"
#include <memory>
#include "AppInfo.h"
#include "RMObject.h"

using namespace DirectX;


struct RMObjectCollectionData
{
	float Count;
	V3 __padding;

	RMObjectData Objects[256];
};

struct LightData
{
	V3 LightDirection;
	float LightIntensity;
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
};

struct CameraData
{
	M4X4 ViewMatrix;
	V3 Position;
	float FOV;
};

struct PerRenderingData
{
	float Time;
	float Width;
	float Height;
	float _padding;

	CameraData CameraData;

	LightData LightData;

	RMObjectCollectionData Objects;
};

enum class Operation
{
	Union = 0,
	Intersection = 1,
	Difference = 2
};

class PerRenderingDataContainer
{

protected:
	InitResult CreateDX11Buffer(ID3D11Device* pD3DDevice);

public:

	InitResult Initialize(ID3D11Device* d3dDevice);
	void DeInitialize();

	void Clear();
	void Bind(ID3D11DeviceContext* pD3DDeviceCOntext);

	void SetTime(float time);
	void SetResolution(float width, float height);
	void SetCameraData(float fov,V3 position, M4X4* view);
	void SetLightData(LightData data);
	void AddObject(RMObjectData data );

private:

	int _objectIndex = 0;
	std::unique_ptr<PerRenderingData> _data;

	ID3D11Buffer* _buffer = nullptr;
};