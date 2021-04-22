#include "RayMarchingUtil.hlsl"

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 positionCS :TEXCOORD1;
};



float4 main(PixelInput IN) : SV_TARGET
{
	float3 viewDir = RayDirection(Camera.FOV, IN.positionCS.xy, float2(Width,Height));
	
	float3 forward = mul(Camera.ViewMatrix, float4(viewDir, 0.0)).xyz;

	float depth = RayMarch(Camera.Position, forward, 0, 10000);

		// Didn't hit anything
	if (depth > 10000 - EPSILON)
	{
		return 0;
	}

	float3 contactPoint = Camera.Position + depth * forward;
	float3 normalizedLight = normalize(Light.LightDirection);
	float3 normalizedNormal = SDF_EstimateNormal(Camera.Position + depth * forward);

	// diffuse light
	float diffuse = dot(-normalizedLight, normalizedNormal); // calculate light intensity
	diffuse = max(diffuse, 0.0f); // dot product can be negative
	diffuse *= Light.LightIntensity; // adjust light intensity by multiplicator

	float4 color = float4(saturate(Light.AmbientColor + Light.DiffuseColor * diffuse),1);
	return color;

	return 1;
}