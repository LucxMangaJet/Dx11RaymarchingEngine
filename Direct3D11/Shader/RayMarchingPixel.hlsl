#include "RayMarchingUtil.inc"

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 positionCS :TEXCOORD1;
};

float4 main(PixelInput IN) : SV_TARGET
{
	float3 viewDir = RayDirection(Camera.FOV, IN.positionCS.xy, float2(Width,Height));
	
	float3 forward = mul(Camera.ViewMatrix, float4(viewDir, 0.0)).xyz;
	float MAX_DIST = 1000;

	float depth = RayMarch(Camera.Position, forward, 0, MAX_DIST);

		// Didn't hit anything
	if (depth > MAX_DIST - EPSILON)
	{
		discard;
	}

	float3 contactPoint = Camera.Position + depth * forward;
	float3 normalizedLight = normalize(Light.LightDirection);
	float3 normalizedNormal = SDF_EstimateNormal(contactPoint, depth);

	//Shadows should work something like this, does not make sense in infinitely repeated context
	/*
	float3 lightOrigin = contactPoint - normalizedLight * (MAX_DIST-1);
	float shadowDepth = RayMarch(lightOrigin, normalizedLight, 0, MAX_DIST);
	float hasShadow = step(MAX_DIST - EPSILON, shadowDepth);
	*/

	//diffuse
	float diffuse = dot(-normalizedLight, normalizedNormal); 
	diffuse = max(diffuse, 0.0f); 

	//specular
	float3 h = normalize(normalize(Camera.Position - contactPoint) - normalizedLight);
	float specular = pow(saturate(dot(h, normalizedNormal)),Light.SpecularPower);

	float3 specularColor = Light.SpecularColor * specular * Light.SpecularIntensity;
	float3 diffuseColor = Light.LightIntensity * Light.DiffuseColor.rgb * diffuse;

	float4 color = float4(saturate(Light.AmbientColor + specularColor + diffuseColor),1);
	return color;
}