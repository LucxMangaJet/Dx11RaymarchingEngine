#include "ExtraMath.h"

V3 operator+(const V3& a, const V3& b)
{
	return V3(a.x + b.x, a.y + b.y, a.z + b.z);
}

V3 operator*(const V3& a, const float& s)
{
	return V3(a.x * s, a.y * s, a.z * s);
}

V3 operator*(const float& s, const V3& a)
{
	return V3(a.x * s, a.y * s, a.z * s);
}

float Magnitude(const V3& a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

V3 Normalize(const V3& a)
{
	return a * (1 / Magnitude(a));
}