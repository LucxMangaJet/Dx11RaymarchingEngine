#pragma once
#include "GUIBase.h"

class PlayerController;
class Camera;

class GUIPlayerController : public GUIBase
{
public:
	void Init(const AppInfo& appInfo);
	virtual void Draw(const AppInfo& appInfo) override;

private:
	PlayerController* _playerController;
	Camera* _camera;
	bool _isUnfolded;
};

