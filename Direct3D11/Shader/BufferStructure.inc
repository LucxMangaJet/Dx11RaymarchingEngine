#define MAX_OBJECTS 256

struct RMObject
{
	float3 Scale;
	float Type;

	float Operation;
	float3 Repetition;

	float4x4 ITRMatrix;
};


struct RMObjectCollectionData
{
	float Count;
	float3 __padding;

	RMObject Objects[MAX_OBJECTS];
};

struct LightData
{
	float3 LightDirection;
	float LightIntensity;
	float4 AmbientColor;
	float4 DiffuseColor;
	float4 SpecularColor;
	float SpecularPower;
	float SpecularIntensity;
	float2 __padding;
};

struct CameraData
{
	float4x4 ViewMatrix;
	float3 Position;
	float FOV;
};

cbuffer PerRenderingData :register(b0)
{
	float Time;
	float Width;
	float Height;
	float _padding;

	CameraData Camera;

	LightData Light;

	RMObjectCollectionData ObjectsCollection;
};
