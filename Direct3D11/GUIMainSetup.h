#pragma once
#include "GUIBase.h"
#include "GUIConsole.h"
#include "GUIMenu.h"
#include "GUIInfo.h"

class GUIMainSetup : public GUIBase
{

public:

	InitResult Init(const AppInfo& appInfo);

	virtual void Draw(const AppInfo& appInfo) override;

	private:

		void OnGuiMenuEvent(GUIMenuEventType type);

		GUIConsole _console;
		GUIMenu _menu;
		GUIInfo _info;
		bool _showDemo;
};

