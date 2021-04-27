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
#include <vector>
#include "RMObject.h"

struct  LightData;

class Engine
{

public:

	InitResult Initialize(const AppInfo& appInfo);

	void Update(const AppInfo& appInfo);
	void Render(const AppInfo& appInfo);

	PlayerController* GetPlayerController() { return &_playerController; }
	Camera* GetActiveCamera() { return &_camera; }

	std::vector<RMObject*> const * const GetAllObjects() { return &_objects; }

	RMObject* CreateObject(RMObjectType type, std::string name = std::string("Unnamed"),
		V3 position = V3(), V3 eularAngles = V3(), V3 scale = V3(1, 1, 1),
		V3 repetition = V3(), RMOperation operation = RMOperation::Union);

	void DeInitialize();

protected:

	PerRenderingDataContainer _perRenderData;
	Camera _camera;

	Material _mainMaterial;
	Material _skyboxMaterial;
	
	Mesh _renderPlane;
	LightData _light;

	GameObject _raymarchObject;
	GameObject _skyboxObject;

	PlayerController _playerController;

	std::vector<RMObject*> _objects;
};
