#pragma once
#include "GUIBase.h"
#include "imgui.h"
#include "GUIPLayerController.h"
#include "GUIRMObject.h"

struct  AppInfo;

class GUIInfo :public GUIBase
{
public:
	void Init(const AppInfo& appInfo);
	virtual void Draw(const AppInfo& info) override;

private:
	ImVec2 _position { 0,0 };

	GUIObjects guiObjects;
	GUIPLayerController guiPLayerController;
};

