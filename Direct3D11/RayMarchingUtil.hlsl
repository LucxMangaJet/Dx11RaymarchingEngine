
#define MAX_MARCHING_STEPS 128
#define EPSILON 0.001
#define MAX_OBJECTS 256

struct RMObject
{
	float3 Scale;
	float Type;
	float Operation;
	float3 Modulo;
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
	float3 AmbientColor;
	float3 DiffuseColor;
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

float SDF_Sphere(float3 p)
{
	return length(p) - 1;
}

float SDF_Cube(float3 p)
{
	// If d.x < 0, then -1 < p.x < 1, and same logic applies to p.y, p.z
	// So if all components of d are negative, then p is inside the unit cube
	float3 d = abs(p) - float3(1.0, 1.0, 1.0);

	// Assuming p is inside the cube, how far is it from the surface?
	// Result will be negative or zero.
	float insideDistance = min(max(d.x, max(d.y, d.z)), 0.0);

	// Assuming p is outside the cube, how far is it from the surface?
	// Result will be positive or zero.
	float outsideDistance = length(max(d, 0.0));

	return insideDistance + outsideDistance;
}

float SDF_Union(float a, float b)
{
	return min(a, b);
}

float SDF_Intersect(float a, float b)
{
	return max(a, b);
}

float SDF_Difference(float a, float b)
{
	return max(a, -b);
}

// polynomial smooth min (k = 0.1);
float SDF_SmoothUnion(float a, float b, float k)
{
	float h = max(k - abs(a - b), 0.0) / k;
	return min(a, b) - h * h * k * (1.0 / 4.0);
}

float3 mod3(float3 el, float3 mod)
{
	return float3(el.x % mod.x, el.y % mod.y, el.z % mod.z);
}

// polynomial smooth min (k = 0.1);
float SDF_SmoothCubicUnion(float a, float b, float k)
{
	float h = max(k - abs(a - b), 0.0) / k;
	return min(a, b) - h * h * h * k * (1.0 / 6.0);
}

float SDF_Scene(float3 p)
{
	float res = 10000;

	for (int i = 0; i < ObjectsCollection.Count; i++)
	{
		RMObject object = ObjectsCollection.Objects[i];

		float3 tp = mul(object.ITRMatrix, float4(p, 1)).xyz;
		float3 scale = object.Scale;
		float scalingCorrection = min(scale.x, min(scale.y, scale.z));
		float val = 10000;

		if (length(object.Modulo) == 0)
		{
			if (object.Type == 1)
				val = min(val, SDF_Sphere(tp / scale) * scalingCorrection);
			else if (object.Type == 2)
				val = min(val, SDF_Cube(tp / scale) * scalingCorrection);
		}
		else
		{
			if (object.Type == 1)
				val = min(val, SDF_Sphere(mod3(tp / scale,object.Modulo)) * scalingCorrection);
			else if (object.Type == 2)
				val = min(val, SDF_Cube(mod3(tp / scale,object.Modulo)) * scalingCorrection);
		}


		if (object.Operation == 0)
			res = SDF_Union(res, val);
		else if (object.Operation == 1)
			res = SDF_Intersect(res, val);
		else if (object.Operation == 2)
			res = SDF_Difference(res, val);
	}

	return res;
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


float3 RayDirection(float fieldOfView, float2 clipSpacePosition, float2 size)
{
	float2 xy = clipSpacePosition * size;
	float z = size.y / tan(fieldOfView * 0.5);
	return normalize(float3(xy, z));
}
