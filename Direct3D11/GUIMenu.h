#pragma once
#include "GUIBase.h"
#include <functional>
#include <string>


enum class GUIMenuEventType
{
	Quit,

	Window_Console = 100,
	Window_Info,
	Window_Demo
};

class GUIMenu : public GUIBase
{
public:

	virtual void Draw(const AppInfo& AppInfo) override;

	void SetCallback(std::function<void(GUIMenuEventType)> callback) { _callback = callback; }

private:

	void NotifyCallback(GUIMenuEventType msg) { if (_callback) { _callback(msg); } }

	std::function<void(GUIMenuEventType)> _callback;

};