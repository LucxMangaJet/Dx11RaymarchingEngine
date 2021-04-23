#include "Window.h"
#include "AppInfo.h"

//forward declaration for ImGUI to catch Win32 events. Impl in: ImGUIWin32.cpp
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Main WndProc for MainWindow
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

InitResult Window::Initialize(const AppInfo& appInfo)
{
	WNDCLASS wc = {};
	wc.hInstance = appInfo.HInstance;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND + 1);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.lpszClassName = TEXT("Direct3D 11");
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;

	if (!RegisterClass(&wc)) return InitResult::Failure(10, TEXT("Win32 Failed to register class."));

	INT halfScreenWidth = GetSystemMetrics(SM_CXSCREEN) / 2;
	INT halfScreenHeight = GetSystemMetrics(SM_CYSCREEN) / 2;
	INT halfWidth = appInfo.Width / 2;
	INT halfHeight = appInfo.Height / 2;
	RECT r{ halfScreenWidth - halfWidth, halfScreenHeight - halfHeight, 
			halfScreenWidth + halfWidth, halfScreenHeight + halfHeight };
	DWORD style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&r, style, false);

	_hWnd = CreateWindow(wc.lpszClassName, wc.lpszClassName, style, 
		r.left, r.top, r.right - r.left, r.bottom - r.top, nullptr, nullptr, appInfo.HInstance, nullptr);

	if (!_hWnd) return InitResult::Failure(15, TEXT("Win32 Failed to create window"));

	ShowWindow(_hWnd, appInfo.nCmdShow);
	UpdateWindow(_hWnd);
	SetFocus(_hWnd); 

	return InitResult::Success();
}


bool Window::Run()
{
	MSG msg = {};
	if (PeekMessage(&msg, nullptr, 0, UINT_MAX, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT) return false;
	}
	
	return true;
}

void Window::DeInitialize()
{
	// TODO: destroy window if it is not destroyed
}
