#include "ImGUIWin32.h"

InitResult ImGUIWin32::Initialize(const AppInfo& appInfo)
{
	//query timers to see if they work? Why?

	//if (!::QueryPerformanceFrequency((LARGE_INTEGER*)&g_TicksPerSecond))
	//	return false;
	//if (!::QueryPerformanceCounter((LARGE_INTEGER*)&g_Time))
	//	return false;

	//// Setup backend capabilities flags
	//g_hWnd = (HWND)hwnd;
	//ImGuiIO& io = ImGui::GetIO();
	//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
	//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
	//io.BackendPlatformName = "imgui_impl_win32";
	//io.ImeWindowHandle = hwnd;

	//// Keyboard mapping. Dear ImGui will use those indices to peek into the io.KeysDown[] array that we will update during the application lifetime.
	//io.KeyMap[ImGuiKey_Tab] = VK_TAB;
	//io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	//io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	//io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	//io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	//io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	//io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	//io.KeyMap[ImGuiKey_Home] = VK_HOME;
	//io.KeyMap[ImGuiKey_End] = VK_END;
	//io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
	//io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	//io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	//io.KeyMap[ImGuiKey_Space] = VK_SPACE;
	//io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	//io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	//io.KeyMap[ImGuiKey_KeyPadEnter] = VK_RETURN;
	//io.KeyMap[ImGuiKey_A] = 'A';
	//io.KeyMap[ImGuiKey_C] = 'C';
	//io.KeyMap[ImGuiKey_V] = 'V';
	//io.KeyMap[ImGuiKey_X] = 'X';
	//io.KeyMap[ImGuiKey_Y] = 'Y';
	//io.KeyMap[ImGuiKey_Z] = 'Z';


	////wants me to use xinput for input. Should do. 
	//// Dynamically load XInput library
	//const char* xinput_dll_names[] =
	//{
	//	"xinput1_4.dll",   // Windows 8+
	//	"xinput1_3.dll",   // DirectX SDK
	//	"xinput9_1_0.dll", // Windows Vista, Windows 7
	//	"xinput1_2.dll",   // DirectX SDK
	//	"xinput1_1.dll"    // DirectX SDK
	//};


	//for (int n = 0; n < IM_ARRAYSIZE(xinput_dll_names); n++)
	//	if (HMODULE dll = ::LoadLibraryA(xinput_dll_names[n]))
	//	{
	//		g_XInputDLL = dll;
	//		g_XInputGetCapabilities = (PFN_XInputGetCapabilities)::GetProcAddress(dll, "XInputGetCapabilities");
	//		g_XInputGetState = (PFN_XInputGetState)::GetProcAddress(dll, "XInputGetState");
	//		break;
	//	}

	//return true;
    return InitResult();
}

void ImGUIWin32::Update(const AppInfo& appInfo)
{
}

void ImGUIWin32::Render(const AppInfo& appInfo)
{
}

void ImGUIWin32::DeInitialize()
{
}
