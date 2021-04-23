#pragma once
#include <memory>
#include "Time.h"
#include "PerRenderingDataContainer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "GameObject.h"

struct AppInfo;

class Engine
{

public:

	int init(const AppInfo& appInfo);

	void update(const AppInfo& appInfo);
	void render(const AppInfo& appInfo);

	void deInit();

protected:

	Time _time;
	PerRenderingDataContainer _perRenderData;
	Camera _camera;

	Material _mainMaterial;
	Mesh _renderPlane;

	POINT _oldMousePos;
	GameObject _raymarchObject;
};
