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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	g_AppInfo.HInstance = hInstance;
	g_AppInfo.nCmdShow = nCmdShow;

	g_AppInfo.Width = 800;
	g_AppInfo.Height = 600;
	g_AppInfo.IsWindowed = true;

	int error = 0;

	// Window Setup
	Window window;
	error = window.init(g_AppInfo);
	if (error != 0) return ThrowErrorMSGBox(error);
	g_AppInfo.MainWindow = window.getWindowHandle();
	g_window = &window;

	// D3D11 Setup
	D3D d3d;
	error = d3d.init(g_AppInfo);
	if (error != 0) return ThrowErrorMSGBox(error);

	g_AppInfo.D3DDevice = d3d.getDevice();
	g_AppInfo.D3DDeviceContext = d3d.getDeviceContext();
	g_d3d = &d3d;

	//Engine Setup
	Engine engine;
	engine.init(g_AppInfo);
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
		if (!g_window->run()) break;

		g_engine->update(g_AppInfo);

		// 8.2. draw objects 
		g_d3d->beginScene(0.0f, 0.0f, 0.0f);

		g_engine->render(g_AppInfo);

		g_d3d->endScene();
	}
}

void CleanupApplication()
{
	g_d3d->deInit();
	g_window->deInit();
	g_engine->deInit();
}
