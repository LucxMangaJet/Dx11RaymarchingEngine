#include "RayMarchingUtil.hlsl"

struct RMObject
{
	float3 Position;
	int Type;
	float3 Rotation;
	float Scale;
};

struct RMObjectCollectionData
{
	int Count;
	float3 __padding;

	RMObject Objects[256];
};

struct LightData
{
	float3 LightDirection;
	float LightIntensity;
	float3 AmbientColor;
	float3 DiffuseColor;
};

struct CameraData
{
	float3 ViewProjectionMatrix;
	float FOV;
	float3 __padding;
};

cbuffer PerRenderingData :register(b0)
{
	float Time;
	float Width;
	float Height;
	float _padding;

	CameraData Camera;

	LightData Light;

	RMObjectCollectionData Objects;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 positionCS :TEXCOORD1;
};


float4 main(PixelInput IN) : SV_TARGET
{

	float3 forward = float3(0, 0, 1);
	float m = RayMarch(IN.positionCS, forward, 0 ,100);

	float3 pos = IN.positionCS + m * forward;

	float3 normal = SDF_EstimateNormal(pos);
	if (m > 0)
		return 0;

	return float4(Time,0,0,1);
}