#include "Material.h"
#include <d3dcompiler.h>
#include "Utils.h"
#include <string>
#include "PerRenderingDataContainer.h"


int Material::init(D3D* d3d, LPCWSTR textureName, LPCWSTR vertexShaderName, LPCWSTR pixelShaderName, MaterialParameters parameters)
{
	ID3D11Device* pD3DDevice = d3d->getDevice();

	int error = createVertexShader(pD3DDevice, vertexShaderName);
	if (error != 0) return error;

	error = createPixelShader(pD3DDevice, pixelShaderName);
	if (error != 0) return error;

	return 0;
}

void Material::render(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix, XMFLOAT4X4* viewMatrix, XMFLOAT4X4* projectionMatrix)
{
	pD3DDeviceContext->IASetInputLayout(_pInputLayout);
	pD3DDeviceContext->VSSetShader(_pVertexShader, nullptr, 0);
	pD3DDeviceContext->PSSetShader(_pPixelShader, nullptr, 0);
}

void Material::deInit()
{
	safeRelease<ID3D11VertexShader>(_pVertexShader);
	safeRelease<ID3D11PixelShader>(_pPixelShader);
	safeRelease<ID3D11InputLayout>(_pInputLayout);
}

int Material::createVertexShader(ID3D11Device* pD3DDevice, LPCWSTR name)
{
	ID3DBlob* pCompiledCode = nullptr;
	std::wstring compiledName = std::wstring(name) + L".cso";

	HRESULT hr = D3DReadFileToBlob(compiledName.c_str(), &pCompiledCode);
	//MessageBox(NULL, std::to_wstring(hr).c_str(), name , 0);
	if (FAILED(hr))
	{
		std::wstring rawName = std::wstring(name) + L".hlsl";
		hr = D3DCompileFromFile(
			rawName.c_str(), // shader filename
			nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, // optional macros & includes
			"main", // entry point function name
			"vs_4_0", // shader type & version
			0, 0, // optional flags
			&pCompiledCode, // compiled code target
			nullptr // optional blob for all compile errors
		);

		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			MessageBox(NULL, name, L"File not found", 0);
		}
	}

	if (FAILED(hr)) return 40;

	hr = pD3DDevice->CreateVertexShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_pVertexShader);
	if (FAILED(hr)) return 42;

	int error = createInputLayout(pD3DDevice, pCompiledCode);
	if (error != 0) return error;

	safeRelease<ID3DBlob>(pCompiledCode);

	return 0;
}

int Material::createPixelShader(ID3D11Device* pD3DDevice, LPCWSTR name)
{
	ID3DBlob* pCompiledCode = nullptr;
	ID3DBlob* pCompiledErrors = nullptr;
	std::wstring compiledName = std::wstring(name) + L".cso";

	HRESULT hr = D3DReadFileToBlob(compiledName.c_str(), &pCompiledCode);

	if (FAILED(hr))
	{
		std::wstring rawName = std::wstring(name) + L".hlsl";
		hr = D3DCompileFromFile(
			rawName.c_str(), // shader filename
			nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, // optional macros & includes
			"main", // entry point function name
			"ps_4_0", // shader type & version
			0, 0, // optional flags
			&pCompiledCode, // compiled code target
			&pCompiledErrors // optional blob for all compile errors
		);

		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			MessageBox(NULL, name, L"File not found", 0);
		}
	}
	if (pCompiledErrors)
	{
		char* error = (char*)pCompiledErrors->GetBufferPointer();
		size_t size = strlen(error) + 1;
		wchar_t* res = new wchar_t[size];

		size_t outSize;
		mbstowcs_s(&outSize, res, size, error, size - 1);

		MessageBox(NULL, res, L"Error", 0);
	}
	if (FAILED(hr)) return hr;

	hr = pD3DDevice->CreatePixelShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_pPixelShader);
	if (FAILED(hr)) return 48;

	safeRelease<ID3DBlob>(pCompiledCode);

	return 0;
}

int Material::createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pBlob)
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
	if (FAILED(hr)) return 44;

	return 0;
}


