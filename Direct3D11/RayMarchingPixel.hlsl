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
	float MAX_DIST = 1000;

	float depth = RayMarch(Camera.Position, forward, 0, MAX_DIST);

		// Didn't hit anything
	if (depth > MAX_DIST - EPSILON)
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

	//specular
	float3 h = normalize(normalize(Camera.Position - contactPoint) - normalizedLight);
	float specular = pow(saturate(dot(h, normalizedNormal)), 48.0f);

	float4 color = float4(saturate(Light.AmbientColor + Light.DiffuseColor * diffuse + float3(specular,0,0)),1);
	return color;

	return 1;

	// https://stackoverflow.com/questions/34527505/blinn-phong-shading-with-hlsl-and-d3d11-point-light
	// https://brooknovak.wordpress.com/2008/11/13/hlsl-per-pixel-point-light-using-phong-blinn-lighting-model/
	// Phong relfection is ambient + light-diffuse + spec highlights.
	// I = Ia*ka*Oda + fatt*Ip[kd*Od(N.L) + ks(R.V)^n]
	// Ref: http://www.whisqu.se/per/docs/graphics8.htm
	// and http://en.wikipedia.org/wiki/Phong_shading
}