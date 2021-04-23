#include "Engine.h"
#include "AppInfo.h"
#include "MeshGenerator.h"

#define KEY_W 0x57
#define KEY_S 0x53
#define KEY_D 0x44
#define KEY_A 0x41
#define KEY_PRESSED 0x8000

InitResult Engine::Initialize(const AppInfo& appInfo)
{
	InitResult result;

	// create time
	result = _time.Initialize();
	if (result.Failed) return result;

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

	result = _mainMaterial.Initialize(appInfo.D3DDevice, NULL, L"RayMarchingVertex", L"RayMarchingPixel", parameters1);
	if (result.Failed) return result;

	// 7. create light
	LightData light = {};
	light.LightDirection = { 1.0f, 0.0f, 0.0f };
	light.AmbientColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	light.DiffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	light.LightIntensity = 1.0f;

	//create GameObject
	_raymarchObject = {};
	_raymarchObject.init(appInfo.D3DDevice, &_renderPlane, &_mainMaterial);

	//create per rendering data
	result = _perRenderData.Initialize(appInfo.D3DDevice);
	if (result.Failed) return result;
	_perRenderData.SetResolution(appInfo.Width, appInfo.Height);
	_perRenderData.SetLightData(light);

	_perRenderData.AddObject(4, XMFLOAT3(0, 0, 5), XMFLOAT3(), XMFLOAT3(10, 10, 10), Operation::Union);
	_perRenderData.AddObject(1, XMFLOAT3(0, -3, 5), XMFLOAT3(), XMFLOAT3(1, 1, 1), Operation::Union);
	//perRenderingData.AddObject(1, XMFLOAT3(0, 0, 5), XMFLOAT3(), XMFLOAT3(1.5, 1.5, 1.5), Operation::Difference);
	//perRenderingData.AddObject(1, XMFLOAT3(2, 0, 5), XMFLOAT3(0, 1, 0), XMFLOAT3(1, 1, 1));
	//perRenderingData.AddObject(2, XMFLOAT3(3, 0, 5), XMFLOAT3(0, 2, 0), XMFLOAT3(1, 1, 1), Operation::Difference);

	_oldMousePos = {};
	_oldMousePos.x = appInfo.Width * 0.5f;
	_oldMousePos.y = appInfo.Height * 0.5f;
	return InitResult::Success();
}

void Engine::Update(const AppInfo& appInfo)
{
	_time.Update();

	POINT mousePos;
	if (GetCursorPos(&mousePos))
	{
		POINT diff;
		diff.x = mousePos.x - _oldMousePos.x;
		diff.y = mousePos.y - _oldMousePos.y;

		XMFLOAT3 eular = _camera.GetWorldRotation();
		eular.y += (float)diff.x / appInfo.Width;
		eular.x += (float)diff.y / appInfo.Height;
		_camera.SetEulerAngles(eular);
		_oldMousePos = mousePos;

	}

	XMFLOAT3 f_position = _camera.GetWorldPosition();
	XMFLOAT3 f_forward = _camera.GetForwardVector();
	XMFLOAT3 f_right = _camera.GetRightVector();

	XMVECTOR position = XMLoadFloat3(&f_position);
	XMVECTOR forward = XMLoadFloat3(&f_forward);
	XMVECTOR right = XMLoadFloat3(&f_right);

	float deltaTime = _time.GetDeltaTime();

	if (GetKeyState(KEY_W) & KEY_PRESSED)
	{
		position = XMVectorAdd(position, XMVectorScale(forward, deltaTime));
	}
	if (GetKeyState(KEY_S) & KEY_PRESSED)
	{
		position = XMVectorAdd(position, XMVectorScale(forward, -deltaTime));
	}
	if (GetKeyState(KEY_A) & KEY_PRESSED)
	{
		position = XMVectorAdd(position, XMVectorScale(right, -deltaTime));
	}
	if (GetKeyState(KEY_D) & KEY_PRESSED)
	{
		position = XMVectorAdd(position, XMVectorScale(right, deltaTime));
	}

	XMStoreFloat3(&f_position, position);
	_camera.SetPosition(f_position);

	_camera.Update();

}

void Engine::Render(const AppInfo& appInfo)
{
	//Setup per rendering buffer
	_perRenderData.SetTime(_time.GetTotalTime());
	_perRenderData.SetCameraData(_camera.GetFOV(), _camera.GetWorldPosition(), _camera.GetViewMatrix());
	_perRenderData.Bind(appInfo.D3DDeviceContext);

	// rendering stuff
	_raymarchObject.render(appInfo.D3DDeviceContext);
}

void Engine::DeInitialize()
{

	_mainMaterial.DeInitialize();
	_time.DeInitialize();
	_camera.DeInitialize();
	_renderPlane.DeInitialize();
	_perRenderData.DeInitialize();
}
