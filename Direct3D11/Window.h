#pragma once
#include <Windows.h>

struct AppInfo;

class Window
{
public:
	int init(const AppInfo& appInfo);
	bool run();
	void deInit();

	HWND getWindowHandle() { return _hWnd; }

private:
	HWND _hWnd = nullptr;
};

