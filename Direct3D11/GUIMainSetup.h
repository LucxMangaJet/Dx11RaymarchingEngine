#pragma once
#include "GUIBase.h"
#include "GUIConsole.h"
#include "GUIMenu.h"
#include "GUISettings.h"


class GUIMainSetup : public GUIBase
{

public:

	InitResult Initialize(const AppInfo& appInfo);

	virtual void Draw(const AppInfo& appInfo) override;

	private:

		void OnGuiMenuEvent(GUIMenuEventType type);

		GUIConsole _console;
		GUIMenu _menu;
		GUISettings _info;
		bool _showDemo;
};

