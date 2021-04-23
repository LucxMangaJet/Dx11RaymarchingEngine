#pragma once
#include "AppInfo.h"
#include "imgui.h"

class ImGUIWin32
{
public:
	InitResult Initialize(const AppInfo& appInfo);

	void Update(const AppInfo& appInfo);

	void DeInitialize();

};

