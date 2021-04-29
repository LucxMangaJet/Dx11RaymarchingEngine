#pragma once
#include <string>
#include "AppInfo.h"

struct RMObjectData
{
	V3 Scale;
	float Type;
	float Operation;
	V3 Repetition;
	M4X4 TranslationRotationMatrix;
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

	STRING Name;

	V3 Position;
	V3 EularAngles;
	V3 Scale;

	V3 Repetition;

	RMObjectType Type;
	RMOperation Operation;

};

