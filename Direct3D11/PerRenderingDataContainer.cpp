#include "PerRenderingDataContainer.h"
#include "Material.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>

InitResult PerRenderingDataContainer::Initialize(ID3D11Device* d3dDevice)
{
	_data = std::make_unique<PerRenderingData>();

	InitResult result = CreateDX11Buffer(d3dDevice);
	return result;
}

void PerRenderingDataContainer::DeInitialize()
{
	SafeRelease<ID3D11Buffer>(_buffer);
}

void PerRenderingDataContainer::Clear()
{
	_data->Objects.Count = 0;
	_objectIndex = 0;
}

void PerRenderingDataContainer::Bind(ID3D11DeviceContext* pD3DDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	PerRenderingData* pBuffer = reinterpret_cast<PerRenderingData*>(data.pData);

	*pBuffer = *_data;

	pD3DDeviceContext->Unmap(_buffer, 0);

	pD3DDeviceContext->PSSetConstantBuffers(RMB_PER_RENDERING, 1, &_buffer);
}

void PerRenderingDataContainer::SetTime(float time)
{
	_data->Time = time;
}

void PerRenderingDataContainer::SetResolution(float width, float height)
{
	_data->Height = height;
	_data->Width = width;
}

void PerRenderingDataContainer::SetCameraData(float fov, V3 position, M4X4* view)
{
	CameraData data = {};
	data.FOV = fov;
	data.ViewMatrix = *view;
	data.Position = position;

	_data->CameraData = data;
}

void PerRenderingDataContainer::SetLightData(LightData data)
{
	_data->LightData = data;
}


void PerRenderingDataContainer::AddObject(RMObjectData data)
{
	if (_objectIndex >= 255)
	{
		throw std::exception("Error, too many objects");
	}

	_data->Objects.Objects[_objectIndex] = data;

	_objectIndex++;

	_data->Objects.Count = (float) _objectIndex;
}

InitResult PerRenderingDataContainer::CreateDX11Buffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(PerRenderingData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_buffer);
	if (FAILED(hr)) return InitResult::Failure(hr, TEXT("Dx1:: Failed to create constant buffer."));

	return InitResult::Success();
}