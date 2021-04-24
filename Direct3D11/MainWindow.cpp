#include "MainWindow.h"
#include "AppInfo.h"

//global pointer to get WndProc to understand the reference correctly, should change!
MainWindow* g_mainWindow;

//forward declaration for ImGUI to catch Win32 events. Impl in: ImGUIWin32.cpp
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Main WndProc for MainWindow
LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			g_mainWindow->OnResize(width, height);
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

InitResult MainWindow::Initialize(const AppInfo& appInfo)
{
	g_mainWindow = this;

	WNDCLASS wc = {};
	wc.hInstance = appInfo.HInstance;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND + 1);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.lpszClassName = TEXT("Direct3D 11");
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = MainWindow::WndProc;

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
		r.left, r.top, r.right - r.left, r.bottom - r.top, nullptr, nullptr, appInfo.HInstance, this);

	if (!_hWnd) return InitResult::Failure(15, TEXT("Win32 Failed to create window"));

	ShowWindow(_hWnd, appInfo.nCmdShow);
	UpdateWindow(_hWnd);
	SetFocus(_hWnd);

	return InitResult::Success();
}


bool MainWindow::Run()
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

void MainWindow::DeInitialize()
{
	// TODO: destroy window if it is not destroyed
}

void MainWindow::OnResize(UINT width, UINT height)
{
	if (_onResizeCallback)
		_onResizeCallback(width, height);

}
