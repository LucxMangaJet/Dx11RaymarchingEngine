#pragma once
#include "AppInfo.h"
#include "imgui.h"
#include "ImGUIWin32.h"
#include "ImGUIDirect3D.h"

class ImGUIFacade
{

public:
	InitResult Initialize(const AppInfo& appInfo);

	void Update(const AppInfo& appInfo);
	void Render(const AppInfo& appInfo);

	void DeInitialize();

protected:

	ImGUIWin32 _win32Helper;
	ImGUIDirect3D _direct3DHelper;

};

