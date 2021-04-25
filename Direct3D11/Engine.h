#pragma once
#include <memory>
#include "Time.h"
#include "PerRenderingDataContainer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "GameObject.h"
#include "AppInfo.h"
#include "PlayerController.h"

class Engine
{

public:

	InitResult Initialize(const AppInfo& appInfo);

	void Update(const AppInfo& appInfo);
	void Render(const AppInfo& appInfo);

	PlayerController* GetPlayerController() { return &_playerController; }

	void DeInitialize();

protected:

	PerRenderingDataContainer _perRenderData;
	Camera _camera;

	Material _mainMaterial;
	Mesh _renderPlane;

	GameObject _raymarchObject;
	PlayerController _playerController;
};
