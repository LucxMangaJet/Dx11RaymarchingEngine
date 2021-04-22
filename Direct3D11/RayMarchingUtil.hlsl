
#define MAX_MARCHING_STEPS 5
#define EPSILON 0.001

float SDF_Sphere(float3 p)
{
	return length(p) - 1;
}

float SDF_Scene(float3 p)
{
    return SDF_Sphere(p);
}

float3 SDF_EstimateNormal(float3 p)
{
    return normalize(float3(
        SDF_Scene(float3(p.x + EPSILON, p.y, p.z)) - SDF_Scene(float3(p.x - EPSILON, p.y, p.z)),
        SDF_Scene(float3(p.x, p.y + EPSILON, p.z)) - SDF_Scene(float3(p.x, p.y - EPSILON, p.z)),
        SDF_Scene(float3(p.x, p.y, p.z + EPSILON)) - SDF_Scene(float3(p.x, p.y, p.z - EPSILON))
    ));
}

float RayMarch(float3 position, float3 forward, float min, float max)
{
	float depth = min;
	for (int i = 0; i < MAX_MARCHING_STEPS; i++)
	{
		float dist = SDF_Scene(position + depth * forward);

		if (dist < EPSILON)
		{
			return depth;
		}
		depth += dist;

		if (depth >= max)
		{
			return max;
		}
	}
	return max;
}

float3 rayDirection(float fieldOfView, float2 clipSpacePosition, float2 size)
{
	float z = - size.y / tan(fieldOfView * 0.5);
	return normalize(float3(clipSpacePosition, z));
}
