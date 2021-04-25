#pragma once
#include "AppInfo.h"

class GUIBase
{
protected:
	bool _isShown;


public:
	virtual void Draw(const AppInfo& appInfo) = 0;
	void Show() { _isShown = true; }
	void Hide() { _isShown = false; }
	void Toggle() { _isShown = !_isShown; }
};

