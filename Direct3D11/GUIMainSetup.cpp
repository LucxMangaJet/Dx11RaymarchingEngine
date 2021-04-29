#include "GUIMainSetup.h"
#include <functional>


InitResult GUIMainSetup::Initialize(const AppInfo& appInfo)
{

	_menu.Show();
	_menu.SetCallback([this](GUIMenuEventType t) {OnGuiMenuEvent(t); });

	InitResult result = _console.Initialize(appInfo);
	if (result.Failed) return result;

	_console.Show();

	_info.Init(appInfo);
	_info.Show();
	return InitResult::Success();
}

void GUIMainSetup::Draw(const AppInfo& appInfo)
{
	if (!_isShown) return;

	_menu.Draw(appInfo);
	_console.Draw(appInfo);
	_info.Draw(appInfo);

	if (_showDemo)
		ImGui::ShowDemoWindow(&_showDemo);
}

void GUIMainSetup::OnGuiMenuEvent(GUIMenuEventType type)
{
	switch (type)
	{
	case GUIMenuEventType::Quit:
		PostQuitMessage(0);
		break;
	case GUIMenuEventType::Window_Console:
		_console.Toggle();
		break;
	case GUIMenuEventType::Window_Info:
		_info.Toggle();
		break;
	case GUIMenuEventType::Window_Demo:
		_showDemo != _showDemo;
		break;
	default:
		break;
	}

}
