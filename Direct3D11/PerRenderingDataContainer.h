#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include "D3D.h"

using namespace DirectX;

struct RMObject
{
	XMFLOAT3 Scale;
	float Type;
	float Operation;
	XMFLOAT3 Modulo;
	XMFLOAT4X4 TranslationRotationMatrix;
};

struct RMObjectCollectionData
{
	float Count;
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
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT3 Position;
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
	int createDX11Buffer(ID3D11Device* pD3DDevice);

public:

	int init(D3D* d3d);
	void deinit();

	void clear();
	void bind(ID3D11DeviceContext* pD3DDeviceCOntext);

	void SetTime(float time);
	void SetResolution(float width, float height);
	void SetCameraData(float fov,XMFLOAT3 position, XMFLOAT4X4* view);
	void SetLightData(LightData data);
	void ResetObjects();
	void AddObject(int type, XMFLOAT3 position, XMFLOAT3 eulerAngles, XMFLOAT3 scale, Operation operation = Operation::Union, XMFLOAT3 modulo = XMFLOAT3() );

private:

	int _objectIndex = 0;
	PerRenderingData _data = {};
	ID3D11Buffer* _buffer = nullptr;
};