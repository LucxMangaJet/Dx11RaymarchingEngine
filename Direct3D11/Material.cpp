#include "Material.h"
#include <d3dcompiler.h>
#include <string>
#include "PerRenderingDataContainer.h"
#include "AppInfo.h"
#include "ShaderHandler.h"

InitResult Material::Initialize(const AppInfo& appInfo, LPCSTR vertexShaderName, LPCSTR pixelShaderName)
{

	InitResult result = CreateVertexShader(appInfo, vertexShaderName);
	if (result.Failed)return result;

	result = CreatePixelShader(appInfo, pixelShaderName);
	if (result.Failed)return result;

	return InitResult::Success();
}

void Material::Render(ID3D11DeviceContext* pD3DDeviceContext, M4X4* worldMatrix)
{
	pD3DDeviceContext->IASetInputLayout(_pInputLayout);
	pD3DDeviceContext->VSSetShader(_pVertexShader, nullptr, 0);
	pD3DDeviceContext->PSSetShader(_pPixelShader, nullptr, 0);
}

void Material::DeInitialize()
{
	SafeRelease<ID3D11VertexShader>(_pVertexShader);
	SafeRelease<ID3D11PixelShader>(_pPixelShader);
	SafeRelease<ID3D11InputLayout>(_pInputLayout);
}



InitResult Material::CreateVertexShader(const AppInfo& appInfo, LPCSTR name)
{
	ID3DBlob* compiledCode = appInfo.ShaderHander->GetShader(name);

	if (compiledCode == nullptr) return InitResult::Failure(300, "Vertex Shader not found.");

	HRESULT hr = appInfo.D3DDevice->CreateVertexShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &_pVertexShader);
	if (FAILED(hr)) return InitResult::Failure(hr, "Material: Failed to create vertex shader.");

	InitResult result = CreateInputLayout(appInfo.D3DDevice, compiledCode);
	if (result.Failed) return result;

	SafeRelease<ID3DBlob>(compiledCode);

	return InitResult::Success();
}

InitResult Material::CreatePixelShader(const AppInfo& appInfo, LPCSTR name)
{
	ID3DBlob* pCompiledCode = appInfo.ShaderHander->GetShader(name);

	if (pCompiledCode == nullptr) return InitResult::Failure(301, "Pixel Shader not found.");

	HRESULT hr = appInfo.D3DDevice->CreatePixelShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_pPixelShader);
	if (FAILED(hr)) InitResult::Failure(hr, "Material: Failed to create pixel shader.");

	SafeRelease<ID3DBlob>(pCompiledCode);

	return InitResult::Success();
}

InitResult Material::CreateInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pBlob)
{
	D3D11_INPUT_ELEMENT_DESC elements[5] = {};

	// position
	elements[0].SemanticName = "POSITION";
	elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	// color
	elements[1].SemanticName = "COLOR";
	elements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// normal
	elements[2].SemanticName = "NORMAL";
	elements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// uv
	elements[3].SemanticName = "TEXCOORD";
	elements[3].SemanticIndex = 0;
	elements[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	elements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	elements[4].SemanticName = "TEXCOORD";
	elements[4].SemanticIndex = 1;
	elements[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elements[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;


	HRESULT hr = pD3DDevice->CreateInputLayout(elements, 5, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &_pInputLayout);
	if (FAILED(hr)) return InitResult::Failure(hr, "Material: Failed to create input layout.");

	return InitResult::Success();
}


