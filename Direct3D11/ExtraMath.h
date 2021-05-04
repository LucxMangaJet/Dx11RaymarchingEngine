#pragma once

#include "AppInfo.h"


//Custom Vector operations, because XMVECTOR is not productive and painful to use
V3 operator+(const V3& a, const V3& b);

V3 operator*(const V3& a, const float& s);

V3 operator*(const float& s, const V3& a);

float Magnitude(const V3& a);

V3 Normalize(const V3& a);