#pragma once
#include "GUIBase.h"

class PlayerController;

class GUIPLayerController : public GUIBase
{
public:
	void Init(const AppInfo& appInfo);
	virtual void Draw(const AppInfo& appInfo) override;

private:
	PlayerController* _playerController;

};

