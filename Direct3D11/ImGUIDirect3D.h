#pragma once
#include "AppInfo.h"
#include "imgui.h"

class ImGUIDirect3D
{
public:

	InitResult Initialize(const AppInfo& appInfo);

	void Update(const AppInfo& appInfo);
	void Render(const AppInfo& appInfo, ImDrawData* draw_data);

	void DeInitialize();
};

