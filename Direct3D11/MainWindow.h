#pragma once
#include <Windows.h>
#include "AppInfo.h"
#include <functional>

class MainWindow
{
public:
	InitResult Initialize(const AppInfo& appInfo);
	bool Run();
	void DeInitialize();

	void SetOnResizeCallback(std::function<void(UINT, UINT)> func) { _onResizeCallback = func; }
	void OnResize(UINT width, UINT height);

	HWND getWindowHandle() { return _hWnd; }

private:
	HWND _hWnd = nullptr;
	std::function<void(UINT, UINT)> _onResizeCallback;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

