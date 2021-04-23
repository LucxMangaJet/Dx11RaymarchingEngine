#include <Windows.h>
#include "Window.h"
#include "D3D.h"
#include <string>
#include "AppInfo.h"
#include "Engine.h"

static AppInfo g_AppInfo; //Contains global information and pointers to commonly used objects for initialization (Dx11 & WinApi)
static D3D* g_d3d;
static Window* g_window;
static Engine* g_engine;

void RunMainLoop();
void CleanupApplication();
bool FailedInit(InitResult result);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	g_AppInfo.HInstance = hInstance;
	g_AppInfo.nCmdShow = nCmdShow;

	g_AppInfo.Width = 800;
	g_AppInfo.Height = 600;
	g_AppInfo.IsWindowed = true;

	InitResult result;

	// Window Setup
	Window window;
	result = window.Initialize(g_AppInfo);
	if (FailedInit(result)) return result.ErrorCode;

	g_AppInfo.MainWindow = window.getWindowHandle();
	g_window = &window;

	// D3D11 Setup
	D3D d3d;
	result = d3d.Initialize(g_AppInfo);
	if (FailedInit(result)) return result.ErrorCode;

	g_AppInfo.D3DDevice = d3d.getDevice();
	g_AppInfo.D3DDeviceContext = d3d.getDeviceContext();
	g_d3d = &d3d;

	//Engine Setup
	Engine engine;
	result = engine.Initialize(g_AppInfo);
	if (FailedInit(result)) return result.ErrorCode;

	g_engine = &engine;

	RunMainLoop();

	CleanupApplication();
	return 0;
}

void RunMainLoop()
{

	// 8. run application
	while (true)
	{
		if (!g_window->Run()) break;

		g_engine->Update(g_AppInfo);

		// 8.2. draw objects 
		g_d3d->beginScene(0.0f, 0.0f, 0.0f);

		g_engine->Render(g_AppInfo);

		g_d3d->endScene();
	}
}

void CleanupApplication()
{
	g_d3d->DeInitialize();
	g_window->DeInitialize();
	g_engine->DeInitialize();
}


bool FailedInit(InitResult result)
{
	if (result.Failed)
	{
		std::wstringstream stream;
		stream << TEXT("Init fail: I:");
		stream << result.ErrorCode;
		stream << TEXT(" H: ");
		stream << std::hex << result.ErrorCode;

		stream << TEXT(" ");

		if (result.ErrorMsg)
		{
			stream << result.ErrorMsg;
		}
		stream << std::endl;
		MessageBox(NULL, stream.str().c_str(), TEXT("Init Error"), 0);
		
		return true;
	}

	return false;
}
