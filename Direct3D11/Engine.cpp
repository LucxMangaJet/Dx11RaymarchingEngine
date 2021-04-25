#include "Engine.h"
#include "AppInfo.h"
#include "MeshGenerator.h"
#include "RMObject.h"


InitResult Engine::Initialize(const AppInfo& appInfo)
{
	InitResult result;

	//create render plane
	MeshData sphereData = MeshGenerator::GenerateFace();
	result = _renderPlane.Initialize(appInfo.D3DDevice, &sphereData);
	if (result.Failed) return result;

	// create camera
	result = _camera.Initialize(appInfo.Width, appInfo.Height, XM_PI * 0.3333333f, DirectX::XMFLOAT3(0, 0, -5), DirectX::XMFLOAT3(0, 0, 0));
	if (result.Failed) return result;


	// create material
	MaterialParameters parameters1 = {};
	parameters1.Ambient = { 0,0,0,0 };

	result = _mainMaterial.Initialize(appInfo.D3DDevice, NULL, L"Shader/RayMarchingVertex", L"Shader/RayMarchingPixel", parameters1);
	if (result.Failed) return result;

	// 7. create light
	_light.LightDirection = { 1.0f, 0.0f, 0.0f };
	_light.AmbientColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	_light.DiffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	_light.LightIntensity = 1.0f;

	//create GameObject
	_raymarchObject = {};
	_raymarchObject.Init(appInfo.D3DDevice, &_renderPlane, &_mainMaterial);

	//create PC
	_playerController = {};
	_playerController.Init(appInfo, &_camera);

	//create per rendering data
	result = _perRenderData.Initialize(appInfo.D3DDevice);
	if (result.Failed) return result;


	CreateObject(RMObjectType::Sphere, std::string("Sphere"), XMFLOAT3(1,0,5));
	CreateObject(RMObjectType::Sierpinski, std::string("Triangle"), XMFLOAT3(0,0,5));

	return InitResult::Success();
}

void Engine::Update(const AppInfo& appInfo)
{
	_playerController.Update(appInfo);

}

void Engine::Render(const AppInfo& appInfo)
{
	//Setup per rendering buffer
	_perRenderData.Clear();

	_perRenderData.SetResolution(appInfo.Width, appInfo.Height);
	_perRenderData.SetLightData(_light);
	_perRenderData.SetTime(appInfo.Time->GetTotalTime());
	_perRenderData.SetCameraData(_camera.GetFOV(), _camera.GetWorldPosition(), _camera.GetViewMatrix());

	for (int i=0; i < _objects.size(); i++)
	{
		_perRenderData.AddObject(_objects[i]->GetObjectData());
	}

	_perRenderData.Bind(appInfo.D3DDeviceContext);

	// rendering stuff
	_raymarchObject.render(appInfo.D3DDeviceContext);
}


RMObject* Engine::CreateObject(RMObjectType type, std::string name, XMFLOAT3 position /*= XMFLOAT3()*/, XMFLOAT3 eularAngles /*= XMFLOAT3()*/, XMFLOAT3 scale /*= XMFLOAT3()*/, XMFLOAT3 repetition /*= XMFLOAT3()*/, RMOperation operation /*= RMOperation::Union*/)
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
