#pragma once
#include "GUIBase.h"
#include "imgui.h"
#include "GUIPLayerController.h"
#include "GUIRMObject.h"
#include "GUILights.h"

struct  AppInfo;

class GUISettings :public GUIBase
{
public:
	void Init(const AppInfo& appInfo);
	virtual void Draw(const AppInfo& info) override;

private:
	ImVec2 _position{ 0,0 };
	ImVec2 _size{ 400,600 };

	GUIObjects _guiObjects;
	GUIPlayerController _guiPLayerController;
	GUILights _lights;
};

