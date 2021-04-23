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

struct InitResult
{
	bool Failed = false;
	int ErrorCode = false;
	const wchar_t* ErrorMsg = nullptr;


	static InitResult Success() { return InitResult(); }
	static InitResult Failure(int errorCode, const wchar_t* errorMsg = nullptr) 
	{
		InitResult res;
		res.Failed = true;
		res.ErrorCode = errorCode;
		res.ErrorMsg = errorMsg;
		return res;
	};

};


