#include "Engine.h"
#include "AppInfo.h"
#include "MeshGenerator.h"



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
	LightData light = {};
	light.LightDirection = { 1.0f, 0.0f, 0.0f };
	light.AmbientColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	light.DiffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	light.LightIntensity = 1.0f;

	//create GameObject
	_raymarchObject = {};
	_raymarchObject.Init(appInfo.D3DDevice, &_renderPlane, &_mainMaterial);

	//create PC
	_playerController = {};
	_playerController.Init(appInfo, &_camera);

	//create per rendering data
	result = _perRenderData.Initialize(appInfo.D3DDevice);
	if (result.Failed) return result;
	_perRenderData.SetResolution(appInfo.Width, appInfo.Height);
	_perRenderData.SetLightData(light);

	_perRenderData.AddObject(4, XMFLOAT3(0, 0, 5), XMFLOAT3(), XMFLOAT3(10, 10, 10), Operation::Union);
	_perRenderData.AddObject(1, XMFLOAT3(0, -3, 5), XMFLOAT3(), XMFLOAT3(1, 1, 1), Operation::Union, XMFLOAT3(20, 20, 20));
	//perRenderingData.AddObject(1, XMFLOAT3(0, 0, 5), XMFLOAT3(), XMFLOAT3(1.5, 1.5, 1.5), Operation::Difference);
	//perRenderingData.AddObject(1, XMFLOAT3(2, 0, 5), XMFLOAT3(0, 1, 0), XMFLOAT3(1, 1, 1));
	//perRenderingData.AddObject(2, XMFLOAT3(3, 0, 5), XMFLOAT3(0, 2, 0), XMFLOAT3(1, 1, 1), Operation::Difference);


	return InitResult::Success();
}

void Engine::Update(const AppInfo& appInfo)
{
	_playerController.Update(appInfo);

}

void Engine::Render(const AppInfo& appInfo)
{
	//Setup per rendering buffer
	_perRenderData.SetTime(appInfo.Time->GetTotalTime());
	_perRenderData.SetCameraData(_camera.GetFOV(), _camera.GetWorldPosition(), _camera.GetViewMatrix());
	_perRenderData.Bind(appInfo.D3DDeviceContext);

	// rendering stuff
	_raymarchObject.render(appInfo.D3DDeviceContext);
}

void Engine::DeInitialize()
{
	_mainMaterial.DeInitialize();
	_camera.DeInitialize();
	_renderPlane.DeInitialize();
	_perRenderData.DeInitialize();
}
