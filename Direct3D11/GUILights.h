#pragma once
#include "GUIBase.h"

class GUILights : public GUIBase
{
public:
	// Inherited via GUIBase
	virtual void Draw(const AppInfo& appInfo) override;

private:
	bool _isUnfolded;
};

