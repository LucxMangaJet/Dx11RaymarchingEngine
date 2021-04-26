#include "BufferStructure.inc"

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 positionCS :TEXCOORD1;
};

float3 RayDirection(float fieldOfView, float2 clipSpacePosition, float2 size)
{
	float2 xy = clipSpacePosition * size;
	float z = size.y / tan(fieldOfView * 0.5);
	return normalize(float3(xy, z));
}

float4 main(PixelInput IN) : SV_TARGET
{
	float3 viewDir = RayDirection(Camera.FOV, IN.positionCS.xy, float2(Width,Height));
	float3 forward = mul(Camera.ViewMatrix, float4(viewDir, 0.0)).xyz;

	float3 col = lerp(float3(1, 0, 1), float3(1, 0, 0), forward.y) * saturate(forward.y);
	return float4(col,1);
}

