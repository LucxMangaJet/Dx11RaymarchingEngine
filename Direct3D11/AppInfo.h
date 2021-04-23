#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <iomanip>
#include <sstream>


class Time;

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
	Time* Time;
};

struct InitResult
{
	bool Failed = false;
	int ErrorCode = false;
	std::wstring ErrorMsg;


	static InitResult Success() { return InitResult(); }
	static InitResult Failure(int errorCode, const wchar_t* errorMsg = nullptr) 
	{
		InitResult res;
		res.Failed = true;
		res.ErrorCode = errorCode;
		if(errorMsg)
			res.ErrorMsg = std::wstring(errorMsg);
		return res;
	};

};


