#pragma once
#include "AppInfo.h"

#define KEY_W 0x57
#define KEY_S 0x53
#define KEY_D 0x44
#define KEY_A 0x41
#define KEY_PRESSED 0x8000

class Camera;

class PlayerController
{

public:

	InitResult Init(const AppInfo& AppInfos, Camera* camera);
	void Update(const AppInfo& appInfo);
	void OnPrePhysics(const AppInfo& appInfo);

	float _mouseSensitivity = 3;
	float _movementSpeed = 1;
	float _vy;

private:
	POINT _oldMousePos;
	Camera* _camera;
	POINT_ID _collisionID;
	V3 _collisionPoints[20];
	float _CollisionPointCount = 20;
};

