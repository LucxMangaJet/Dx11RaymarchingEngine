#include "RayMarchingSDF.hlsl"

#define MAX_MARCHING_STEPS 128
#define EPSILON 0.001
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


float3 SDF_Repeat(float3 p, float3 c)
{
	float3 q = fmod(p + 0.5 * c, c) - 0.5 * c;
	return q;
}



// cubic polynomial smooth min (k = 0.1);
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

		//translation + rotation
		float3 tp = mul(object.ITRMatrix, float4(p, 1)).xyz;
		float3 scale = object.Scale;
		float scalingCorrection = min(scale.x, min(scale.y, scale.z));
		float val = 10000;

		//repetition
		float repeatLen = length(object.Repetition);
		if (repeatLen != 0)
		{
			tp = SDF_Repeat(tp, object.Repetition);
		}

		//type selection + scale
		if (object.Type == 1)
			val = SDF_Sphere(tp / scale) * scalingCorrection;
		else if (object.Type == 2)
			val = SDF_Cube(tp / scale) * scalingCorrection;
		else if (object.Type == 3)
			val = SDF_Mandelbulb(tp / scale) * scalingCorrection;
		else if (object.Type == 4)
			val = SDF_Sierpinski(tp / scale) * scalingCorrection;

		//operation
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
	float step = EPSILON;

	return normalize(float3(
		SDF_Scene(float3(p.x + step, p.y, p.z)) - SDF_Scene(float3(p.x - step, p.y, p.z)),
		SDF_Scene(float3(p.x, p.y + step, p.z)) - SDF_Scene(float3(p.x, p.y - step, p.z)),
		SDF_Scene(float3(p.x, p.y, p.z + step)) - SDF_Scene(float3(p.x, p.y, p.z - step))
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
