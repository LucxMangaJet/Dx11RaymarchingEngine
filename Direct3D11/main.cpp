#include <Windows.h>
#include "Window.h"
#include "D3D.h"
#include "Mesh.h"
#include "Camera.h"
#include "Time.h"
#include "Material.h"
#include "GameObject.h"
#include <string>
#include "MeshGenerator.h"
#include "PerRenderingDataContainer.h"
#include <iomanip>
#include <sstream>

#define KEY_W 0x57
#define KEY_S 0x53
#define KEY_D 0x44
#define KEY_A 0x41
#define KEY_PRESSED 0x8000

std::wstring IntToHexString(int nr)
{
	std::wostringstream oss;  // note the 'w'
	oss << std::hex << nr;
	return oss.str();
}

int ThrowErrorMSGBox(int code)
{
	MessageBox(NULL, (std::to_wstring(code) + L" Hex: " + IntToHexString(code)).c_str(), L"Error", 0);
	return code;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	INT width = 800;
	INT height = 600;
	bool isWindowed = true;

	// 1. create window
	Window window;
	int error = window.init(hInstance, width, height, nCmdShow);
	if (error != 0) return ThrowErrorMSGBox(error);

	// 2. create Direct3D 11 interface
	D3D d3d;
	error = d3d.init(window.getWindowHandle(), width, height, isWindowed);
	if (error != 0) return ThrowErrorMSGBox(error);

	// 3. create mesh
	Mesh face;
	MeshData sphereData = MeshGenerator::GenerateFace();
	error = face.init(d3d.getDevice(), &sphereData);
	if (error != 0) return ThrowErrorMSGBox(error);


	// 4. create camera
	Camera camera;
	error = camera.init(width, height, XM_PI * 0.3333333f, DirectX::XMFLOAT3(0, 0, -5), DirectX::XMFLOAT3(0, 0, 0));
	if (error != 0) return ThrowErrorMSGBox(error);

	// 5. create time
	Time time;
	error = time.init();
	if (error != 0) return ThrowErrorMSGBox(error);

	// 6. create material
	Material raymarchingMaterial;
	MaterialParameters parameters1 = {};
	parameters1.Ambient = { 0,0,0,0 };

	error = raymarchingMaterial.init(&d3d, NULL, L"RayMarchingVertex", L"RayMarchingPixel", parameters1);


	if (error != 0) return ThrowErrorMSGBox(error);

	// 7. create light
	LightData light = {};
	light.LightDirection = { 1.0f, 0.0f, 0.0f };
	light.AmbientColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	light.DiffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	light.LightIntensity = 1.0f;

	//8. GameObject
	GameObject gameObject;
	gameObject.init(d3d.getDevice(), &face, &raymarchingMaterial);

	//per rendering data
	PerRenderingDataContainer perRenderingData;
	perRenderingData.init(&d3d);
	perRenderingData.SetResolution(width, height);
	perRenderingData.SetLightData(light);

	perRenderingData.AddObject(2, XMFLOAT3(0, 0, 5), XMFLOAT3(), XMFLOAT3(1, 1, 1), Operation::Union);
	perRenderingData.AddObject(1, XMFLOAT3(0, 0, 5), XMFLOAT3(), XMFLOAT3(1.5, 1.5, 1.5), Operation::Difference);
	perRenderingData.AddObject(1, XMFLOAT3(2, 0, 5), XMFLOAT3(0, 1, 0), XMFLOAT3(1, 1, 1));
	perRenderingData.AddObject(2, XMFLOAT3(3, 0, 5), XMFLOAT3(0, 2, 0), XMFLOAT3(1, 1, 1), Operation::Difference);

	POINT old_mousePos = {};

	// 8. run application
	while (true)
	{
		if (!window.run()) break;

		// 8.1. update objects
		time.update();

		// 8.2. draw objects 
		d3d.beginScene(0.0f, 0.0f, 0.0f);

		POINT mousePos;
		if (GetCursorPos(&mousePos))
		{
			POINT diff;
			diff.x = mousePos.x - old_mousePos.x;
			diff.y = mousePos.y - old_mousePos.y;

			XMFLOAT3 eular = camera.getWorldRotation();
			eular.y += (float)diff.x / width;
			eular.x += (float)diff.y / height;
			camera.SetEulerAngles(eular);
			old_mousePos = mousePos;

		}

		XMFLOAT3 f_position = camera.getWorldPosition();
		XMFLOAT3 f_forward = camera.getForwardVector();
		XMFLOAT3 f_right = camera.getRightVector();

		XMVECTOR position = XMLoadFloat3(&f_position);
		XMVECTOR forward = XMLoadFloat3(&f_forward);
		XMVECTOR right = XMLoadFloat3(&f_right);

		if (GetKeyState(KEY_W) & KEY_PRESSED)
		{
			position = XMVectorAdd(position, XMVectorScale(forward, time.getDeltaTime()));
		}
		if (GetKeyState(KEY_S) & KEY_PRESSED)
		{
			position = XMVectorAdd(position, XMVectorScale(forward, -time.getDeltaTime()));
		}
		if (GetKeyState(KEY_A) & KEY_PRESSED)
		{
			position = XMVectorAdd(position, XMVectorScale(right, -time.getDeltaTime()));
		}
		if (GetKeyState(KEY_D) & KEY_PRESSED)
		{
			position = XMVectorAdd(position, XMVectorScale(right, time.getDeltaTime()));
		}

		XMStoreFloat3(&f_position, position);
		camera.SetPosition(f_position);

		camera.Update();

		//Setup per rendering buffer
		perRenderingData.SetTime(time.getTotalTime());
		perRenderingData.SetCameraData(camera.getFOV(), camera.getWorldPosition(), camera.getViewMatrix());
		perRenderingData.bind(d3d.getDeviceContext());

		// rendering stuff
		gameObject.render(d3d.getDeviceContext());

		d3d.endScene();
	}

	// 9. tidy up
	raymarchingMaterial.deInit();
	time.deInit();
	camera.deInit();
	face.deInit();
	d3d.deInit();
	perRenderingData.deinit();
	window.deInit();

	return 0;
}
