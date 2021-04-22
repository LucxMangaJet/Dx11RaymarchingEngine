#include "PerRenderingDataContainer.h"
#include "Material.h"
#include "Utils.h"

int PerRenderingDataContainer::init(D3D* d3d)
{
	clear();

	int result = createDX11Buffer(d3d->getDevice());
	if (result != 0) return result;

	return 0;
}

void PerRenderingDataContainer::deinit()
{
	safeRelease<ID3D11Buffer>(_buffer);
}

void PerRenderingDataContainer::clear()
{
	_data = PerRenderingData();
}

void PerRenderingDataContainer::bind(ID3D11DeviceContext* pD3DDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	PerRenderingData* pBuffer = reinterpret_cast<PerRenderingData*>(data.pData);

	*pBuffer = _data;

	pD3DDeviceContext->Unmap(_buffer, 0);

	pD3DDeviceContext->PSSetConstantBuffers(RMB_PER_RENDERING, 1, &_buffer);
}

void PerRenderingDataContainer::SetTime(float time)
{
	_data.Time = time;
}

void PerRenderingDataContainer::SetResolution(float width, float height)
{
	_data.Height = height;
	_data.Width = width;
}

void PerRenderingDataContainer::SetCameraData(float fov, XMFLOAT4X4* view)
{
	CameraData data = {};
	data.FOV = fov;
	data.ViewMatrix = *view;

	_data.CameraData = data;
}

void PerRenderingDataContainer::SetLightData(LightData data)
{
	_data.LightData = data;
}

int PerRenderingDataContainer::createDX11Buffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(PerRenderingData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_buffer);
	if (FAILED(hr)) return 101;

	return 0;
}