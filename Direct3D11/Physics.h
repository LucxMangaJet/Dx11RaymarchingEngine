#pragma once
#include "AppInfo.h"
#include <vector>

struct SphereCollider
{
	V3 Position;
	float Radius;
};



class Physics
{
public:
	InitResult Initiate(const AppInfo& appInfo);
	void Update(const AppInfo& appInfo);


private:
	std::vector<SphereCollider> _colliders;

};

