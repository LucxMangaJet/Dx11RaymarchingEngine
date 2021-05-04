#include <Windows.h>
#include "MainWindow.h"
#include "Direct3D.h"
#include <string>
#include "AppInfo.h"
#include "Engine.h"
#include "ImGUIFacade.h"
#include "Event.h"
#include "GUIMainSetup.h"
#include "loguru.hpp"
#include "ShaderHandler.h"

static AppInfo g_AppInfo; //Contains global information and pointers to commonly used objects for initialization (Dx11 & WinApi)
static Direct3D* g_direct3D;
static MainWindow* g_window;
static Engine* g_engine;
static ImGUIFacade* g_imgui;
static Time* g_time;
static GUIMainSetup* g_gui;
static ShaderHandler* g_shaderHandler;

void RunMainLoop();
void Shutdown();
bool FailedInit(InitResult result);
void OnResize(UINT width, UINT height);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	int i = 0;
	g_AppInfo.HInstance = hInstance;
	g_AppInfo.nCmdShow = nCmdShow;

	g_AppInfo.Width = 1024;
	g_AppInfo.Height = 576;
	g_AppInfo.IsWindowed = true;

	InitResult result;

	//Setup logging
	loguru::g_stderr_verbosity = 1;
	loguru::add_file("player.log", loguru::Truncate, loguru::Verbosity_MAX);
	LOG_F(INFO, "Starting Application");

	// Window Setup
	g_window = new MainWindow();
	result = g_window->Initialize(g_AppInfo);
	if (FailedInit(result)) return result.ErrorCode;

	g_AppInfo.MainWindow = g_window->getWindowHandle();
	g_window->SetOnResizeCallback(OnResize);

	// D3D11 Setup
	g_direct3D = new Direct3D();
	result = g_direct3D->Initialize(g_AppInfo);
	if (FailedInit(result)) return result.ErrorCode;

	g_AppInfo.D3DDevice = g_direct3D->GetDevice();
	g_AppInfo.D3DDeviceContext = g_direct3D->GetDeviceContext();


	//Setup Shaders
	g_shaderHandler = new ShaderHandler();
	g_AppInfo.ShaderHander = g_shaderHandler;

	g_shaderHandler->AddShader("SkyboxVertex", TEXT("Shader/SkyboxVertex.hlsl"), ShaderType::VertexShader);
	g_shaderHandler->AddShader("SkyboxPixel", TEXT("Shader/SkyboxPixel.hlsl"), ShaderType::PixelShader);
	g_shaderHandler->AddShader("Render", TEXT("Shader/Render.compute"), ShaderType::ComputeShader);
	g_shaderHandler->AddShader("RayMarchingVertex", TEXT("Shader/RayMarchingVertex.hlsl"), ShaderType::VertexShader);
	g_shaderHandler->AddShader("RayMarchingPixel", TEXT("Shader/RayMarchingPixel.hlsl"), ShaderType::PixelShader);
	g_shaderHandler->AddShader("Physics", TEXT("Shader/Physics.compute"), ShaderType::ComputeShader);

	result = g_shaderHandler->LoadAllShaders();
	if (FailedInit(result)) return result.ErrorCode;

	//Time Setup
	g_time = new Time();
	result = g_time->Initialize();
	if (FailedInit(result)) return result.ErrorCode;
	g_AppInfo.Time = g_time;

	//Engine Setup
	g_engine = new Engine();
	result = g_engine->Initialize(g_AppInfo);
	if (FailedInit(result)) return result.ErrorCode;
	g_AppInfo.Engine = g_engine;

	//ImGUI Setup
	g_imgui = new ImGUIFacade();
	result = g_imgui->Initialize(g_AppInfo);
	if (FailedInit(result)) return result.ErrorCode;

	//GUI Setup
	g_gui = new GUIMainSetup();
	g_gui->Initialize(g_AppInfo);
	g_gui->Show();

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
		g_imgui->Update(g_AppInfo);

		g_gui->Draw(g_AppInfo);


		// render
		g_direct3D->BeginScene(0.0f, 0.0f, 0.0f);

		g_engine->Render(g_AppInfo);
		g_imgui->Render(g_AppInfo);

		g_direct3D->EndScene();
	}
}

void Shutdown()
{
	g_imgui->DeInitialize();
	g_direct3D->DeInitialize();
	g_window->DeInitialize();
	g_engine->DeInitialize();
	g_shaderHandler->DeInitialize();

	delete g_engine;
	delete g_time;
	delete g_direct3D;
	delete g_window;
	delete g_imgui;
	delete g_gui;
	delete g_shaderHandler;
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
		std::stringstream stream;
		stream << "I:";
		stream << result.ErrorCode;
		stream << " H: ";
		stream << std::hex << result.ErrorCode;

		stream << " ";
		stream << result.ErrorMsg;

		stream << std::endl;

		LOG_F(ERROR, stream.str().c_str());
		MessageBoxA(NULL, stream.str().c_str(), "Init Error", 0);

		return true;
	}

	return false;
}