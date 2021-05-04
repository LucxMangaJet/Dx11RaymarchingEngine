#include "Engine.h"
#include "AppInfo.h"
#include "MeshGenerator.h"
#include "RMObject.h"


InitResult Engine::Initialize(const AppInfo& appInfo)
{
	InitResult result;

	//CS Render - Not ready yet
	//result = _csRendering.Initiate(appInfo, "Render");
	//if (result.Failed) return result;

	//Physics
	result = _physics.Initiate(appInfo, "Physics");
	if (result.Failed) return result;

	//create render plane
	MeshData sphereData = MeshGenerator::GenerateFace();
	result = _renderPlane.Initialize(appInfo.D3DDevice, &sphereData);
	if (result.Failed) return result;

	// create camera
	result = _camera.Initialize(appInfo.Width, appInfo.Height, XM_PI * 0.3333333f, V3(103, 100, 100), V3(0, 0, 0));
	if (result.Failed) return result;

	result = _mainMaterial.Initialize(appInfo, "RayMarchingVertex", "RayMarchingPixel");
	if (result.Failed) return result;

	result = _skyboxMaterial.Initialize(appInfo, "SkyboxVertex", "SkyboxPixel");
	if (result.Failed) return result;

	// 7. create light
	LightData lightData;
	lightData.LightDirection = { 0.0f, -0.1f, 0.0f};
	lightData.AmbientColor = { 0.0f, 0.0f, 0.0f ,1.0f };
	lightData.DiffuseColor = { 1.0f, 1.0f, 1.0f,1.0f };
	lightData.LightIntensity = 1.0f;
	lightData.SpecularColor = { 1.0f,0.0f,0.0f,1.0f };
	lightData.SpecularIntensity = 1.0f;
	lightData.SpecularPower = 48.0f;

	_lights.SetLightData(lightData);

	//create GameObject
	_raymarchObject.Init(appInfo.D3DDevice, &_renderPlane, &_mainMaterial);
	_skyboxObject.Init(appInfo.D3DDevice, &_renderPlane, &_skyboxMaterial);

	//create PC
	_playerController = {};
	_playerController.Init(appInfo, &_camera);

	//create per rendering data
	result = _perRenderData.Initialize(appInfo.D3DDevice);
	if (result.Failed) return result;

	CreateObject(RMObjectType::Mandelbulb, STRING("Bulb"), V3(0, 0, 0), V3(), V3(1, 1, 1), V3(5, 5, 5));

	return InitResult::Success();
}

void Engine::Update(const AppInfo& appInfo)
{
	_playerController.Update(appInfo);
	
	_physics.ResetFrame();

	_physics.SetPoint(_camera.GetWorldPosition());

	_physics.Run(appInfo);
}

void Engine::Render(const AppInfo& appInfo)
{
	//Setup per rendering buffer
	_perRenderData.Clear();

	_perRenderData.SetResolution(appInfo.Width, appInfo.Height);
	_perRenderData.SetLightData(*_lights.GetLightData());
	_perRenderData.SetTime(appInfo.Time->GetTotalTime());
	_perRenderData.SetCameraData(_camera.GetFOV(), _camera.GetWorldPosition(), _camera.GetViewMatrix());

	for (int i = 0; i < _objects.size(); i++)
	{
		_perRenderData.AddObject(_objects[i]->GetObjectData());
	}

	_perRenderData.Bind(appInfo.D3DDeviceContext);

	//Not ready yet
	//_csRendering.Render(appInfo);

	// rendering stuff
	_skyboxObject.render(appInfo.D3DDeviceContext);
	_raymarchObject.render(appInfo.D3DDeviceContext);
}


RMObject* Engine::CreateObject(RMObjectType type, std::string name, V3 position /*= V3()*/, V3 eularAngles /*= V3()*/, V3 scale /*= V3()*/, V3 repetition /*= V3()*/, RMOperation operation /*= RMOperation::Union*/)
{
	RMObject* object = new RMObject();
	object->Name = name;
	object->Type = type;
	object->Position = position;
	object->EularAngles = eularAngles;
	object->Scale = scale;
	object->Repetition = repetition;
	object->Operation = operation;

	_objects.push_back(object);
	return object;
}

void Engine::DeInitialize()
{
	for (int i = 0; i < _objects.size(); i++)
	{
		delete _objects[i];
	}
	_objects.clear();

	_mainMaterial.DeInitialize();
	_camera.DeInitialize();
	_renderPlane.DeInitialize();
	_perRenderData.DeInitialize();
}
