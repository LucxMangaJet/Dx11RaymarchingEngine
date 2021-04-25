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

	std::vector<RMObject*> const * const GetAllObjects() { return &_objects; }

	RMObject* CreateObject(RMObjectType type, std::string name = std::string("Unnamed"),
		XMFLOAT3 position = XMFLOAT3(), XMFLOAT3 eularAngles = XMFLOAT3(), XMFLOAT3 scale = XMFLOAT3(1, 1, 1),
		XMFLOAT3 repetition = XMFLOAT3(), RMOperation operation = RMOperation::Union);

	void DeInitialize();

protected:

	PerRenderingDataContainer _perRenderData;
	Camera _camera;

	Material _mainMaterial;
	Mesh _renderPlane;
	LightData _light;

	GameObject _raymarchObject;
	PlayerController _playerController;

	std::vector<RMObject*> _objects;
};
