#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <iomanip>
#include <sstream>

struct AppInfo
{
	int Width;
	int Height;
	int nCmdShow;
	bool IsWindowed;

	HINSTANCE HInstance;
	HWND MainWindow;
	ID3D11Device* D3DDevice;
	ID3D11DeviceContext* D3DDeviceContext;
};

inline std::wstring IntToHexString(int nr)
{
	std::wostringstream oss;  // note the 'w'
	oss << std::hex << nr;
	return oss.str();
}

inline int ThrowErrorMSGBox(int code)
{
	MessageBox(NULL, (std::to_wstring(code) + L" Hex: " + IntToHexString(code)).c_str(), L"Error", 0);
	return code;
}