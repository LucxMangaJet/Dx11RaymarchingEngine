#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <iomanip>
#include <sstream>
#include <DirectXMath.h>
#include "loguru.hpp"

class Time;
class Engine;
class ShaderHandler;

typedef DirectX::XMFLOAT3 V3;
typedef DirectX::XMFLOAT4X4 M4X4;
typedef std::string STRING;
typedef std::wstring WSTRING;
typedef UINT POINT_ID;

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
	Engine* Engine;
	ShaderHandler* ShaderHander;
};

struct InitResult
{
	bool Failed = false;
	int ErrorCode = false;
	std::string ErrorMsg;


	static InitResult Success() { return InitResult(); }
	static InitResult Failure(int errorCode, LPCSTR errorMsg = nullptr)
	{
		InitResult res;
		res.Failed = true;
		res.ErrorCode = errorCode;
		if (errorMsg)
			res.ErrorMsg = std::string(errorMsg);
		return res;
	};

};

template <class T>
void SafeRelease(T*& obj)
{
	if (obj != nullptr)
	{
		obj->Release();
		obj = nullptr;
	}
}