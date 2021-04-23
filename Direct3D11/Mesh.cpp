#include "Mesh.h"
#include "Vertex.h"
#include "Utils.h"



InitResult Mesh::Initialize(ID3D11Device* pD3DDevice, MeshData* data)
{
	_meshData = data;
	InitResult result;

	result = InitVertexBuffer(pD3DDevice);
	if (result.Failed)return result;

	result = InitIndexBuffer(pD3DDevice);
	if (result.Failed)return result;

	return InitResult::Success();
}

void Mesh::Render(ID3D11DeviceContext* pD3DDeviceContext)
{
	static UINT offset = 0;

	pD3DDeviceContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &_vertexStride, &offset);
	pD3DDeviceContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pD3DDeviceContext->DrawIndexed(_indexCount, 0, 0);
}

void Mesh::DeInitialize()
{
	safeRelease<ID3D11Buffer>(_pVertexBuffer);
	safeRelease<ID3D11Buffer>(_pIndexBuffer);
}

InitResult Mesh::InitVertexBuffer(ID3D11Device* pD3DDevice)
{
	_vertexCount = _meshData->Vertices.size();
	_vertexStride = sizeof(Vertex);

	
	
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = _vertexCount * _vertexStride;
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = &_meshData->Vertices[0];

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pVertexBuffer);
	if (FAILED(hr)) return InitResult::Failure(hr, TEXT("Mesh: Failed to create vertex buffer."));

	return InitResult::Success();
}

InitResult Mesh::InitIndexBuffer(ID3D11Device* pD3DDevice)
{
	_indexCount = _meshData->Indices.size();

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = _indexCount * sizeof(WORD);
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = &_meshData->Indices[0];

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pIndexBuffer);
	if (FAILED(hr)) return InitResult::Failure(hr, TEXT("Mesh: failed to create index buffer"));

	return InitResult::Success();
}
