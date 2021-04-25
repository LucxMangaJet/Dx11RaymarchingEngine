#include <Windows.h>
#include "MainWindow.h"
#include "Direct3D.h"
#include <string>
#include "AppInfo.h"
#include "Engine.h"
#include "ImGUIFacade.h"
#include "Event.h"
#include "GUIConsole.h"

static AppInfo g_AppInfo; //Contains global information and pointers to commonly used objects for initialization (Dx11 & WinApi)
static Direct3D* g_direct3D;
static MainWindow* g_window;
static Engine* g_engine;
static ImGUIFacade* g_gui;
static Time* g_time;

//UI
static GUIConsole* g_console;


void RunMainLoop();
void Shutdown();
bool FailedInit(InitResult result);
void OnResize(UINT width, UINT height);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	g_AppInfo.HInstance = hInstance;
	g_AppInfo.nCmdShow = nCmdShow;

	g_AppInfo.Width = 1024;
	g_AppInfo.Height = 576;
	g_AppInfo.IsWindowed = true;

	InitResult result;

	// Window Setup
	MainWindow window;
	result = window.Initialize(g_AppInfo);
	if (FailedInit(result)) return result.ErrorCode;

	g_AppInfo.MainWindow = window.getWindowHandle();
	g_window = &window;
	window.SetOnResizeCallback(OnResize);

	// D3D11 Setup
	Direct3D d3d;
	result = d3d.Initialize(g_AppInfo);
	if (FailedInit(result)) return result.ErrorCode;

	g_AppInfo.D3DDevice = d3d.GetDevice();
	g_AppInfo.D3DDeviceContext = d3d.GetDeviceContext();
	g_direct3D = &d3d;

	//Time Setup
	Time time;
	result = time.Initialize();
	if (FailedInit(result)) return result.ErrorCode;
	g_time = &time;
	g_AppInfo.Time = g_time;

	//Engine Setup
	Engine engine;
	result = engine.Initialize(g_AppInfo);
	if (FailedInit(result)) return result.ErrorCode;
	g_engine = &engine;

	//ImGUI Setup
	ImGUIFacade gui;
	result = gui.Initialize(g_AppInfo);
	if (FailedInit(result)) return result.ErrorCode;
	g_gui = &gui;

	//GUI Setup
	GUIConsole console = GUIConsole();
	g_console = &console;

	RunMainLoop();

	Shutdown();
	return 0;
}

void RunMainLoop()
{
	while (true)
	{
		//handle messages
		if (!g_window->Run()) break;

		//update
		g_time->Update();
		g_engine->Update(g_AppInfo);
		g_gui->Update(g_AppInfo);

		static bool consoleOpen;
		g_console->Draw(g_AppInfo);

		// render
		g_direct3D->BeginScene(0.0f, 0.0f, 0.0f);

		g_engine->Render(g_AppInfo);
		g_gui->Render(g_AppInfo);

		g_direct3D->EndScene();
	}
}

void Shutdown()
{
	g_gui->DeInitialize();
	g_direct3D->DeInitialize();
	g_window->DeInitialize();
	g_engine->DeInitialize();
}

void OnResize(UINT width, UINT height)
{
	g_AppInfo.Width = width;
	g_AppInfo.Height = height;
	g_direct3D->OnResize(g_AppInfo);
}

bool FailedInit(InitResult result)
{
	if (result.Failed)
	{
		std::wstringstream stream;
		stream << TEXT("I:");
		stream << result.ErrorCode;
		stream << TEXT(" H: ");
		stream << std::hex << result.ErrorCode;

		stream << TEXT(" ");
		stream << result.ErrorMsg;

		stream << std::endl;
		MessageBox(NULL, stream.str().c_str(), TEXT("Init Error"), 0);

		return true;
	}

	return false;
}