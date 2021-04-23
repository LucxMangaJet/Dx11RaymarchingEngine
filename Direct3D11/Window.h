#pragma once
#include <Windows.h>
#include "AppInfo.h"

class Window
{
public:
	InitResult Initialize(const AppInfo& appInfo);
	bool Run();
	void DeInitialize();

	HWND getWindowHandle() { return _hWnd; }

private:
	HWND _hWnd = nullptr;
};

