#pragma once
#include <memory>
#include "Time.h"
#include "PerRenderingDataContainer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "GameObject.h"
#include "AppInfo.h"

class Engine
{

public:

	InitResult Initialize(const AppInfo& appInfo);

	void Update(const AppInfo& appInfo);
	void Render(const AppInfo& appInfo);

	void DeInitialize();

protected:

	PerRenderingDataContainer _perRenderData;
	Camera _camera;

	Material _mainMaterial;
	Mesh _renderPlane;

	POINT _oldMousePos;
	GameObject _raymarchObject;
};
