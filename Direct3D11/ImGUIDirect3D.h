#pragma once
#include "AppInfo.h"
class ImGUIDirect3D
{
public:

	InitResult Initialize(const AppInfo& appInfo);

	void Update(const AppInfo& appInfo);
	void Render(const AppInfo& appInfo);

	void DeInitialize();
};

