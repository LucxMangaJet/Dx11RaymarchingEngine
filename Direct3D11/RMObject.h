#pragma once
#include <DirectXMath.h>
#include <string>

using namespace DirectX;

struct RMObjectData
{
	XMFLOAT3 Scale;
	float Type;
	float Operation;
	XMFLOAT3 Repetition;
	XMFLOAT4X4 TranslationRotationMatrix;
};

enum class RMOperation
{
	Union,
	Intersection,
	Difference
};

enum class RMObjectType
{
	None,
	Sphere,
	Box,
	Mandelbulb,
	Sierpinski,
	Maze
};

class RMObject
{
public:

	RMObject();
	RMObjectData GetObjectData();

	std::string Name;

	XMFLOAT3 Position;
	XMFLOAT3 EularAngles;
	XMFLOAT3 Scale;

	XMFLOAT3 Repetition;

	RMObjectType Type;
	RMOperation Operation;

};

