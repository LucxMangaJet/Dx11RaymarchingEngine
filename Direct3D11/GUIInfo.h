#pragma once
#include "GUIBase.h"
#include "imgui.h"

class GUIInfo :public GUIBase
{
public:
	virtual void Draw(const AppInfo& info) override;

private:
	ImVec2 _position { 0,0 };
};

