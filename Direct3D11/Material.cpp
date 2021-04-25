#include "Material.h"
#include <d3dcompiler.h>
#include <string>
#include "PerRenderingDataContainer.h"


InitResult Material::Initialize(ID3D11Device* pd3dDevice, LPCWSTR textureName, LPCWSTR vertexShaderName, LPCWSTR pixelShaderName, MaterialParameters parameters)
{

	InitResult result = CreateVertexShader(pd3dDevice, vertexShaderName);
	if (result.Failed)return result;

	result = CreatePixelShader(pd3dDevice, pixelShaderName);
	if (result.Failed)return result;

	return InitResult::Success();
}

void Material::Render(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix)
{
	pD3DDeviceContext->IASetInputLayout(_pInputLayout);
	pD3DDeviceContext->VSSetShader(_pVertexShader, nullptr, 0);
	pD3DDeviceContext->PSSetShader(_pPixelShader, nullptr, 0);
}

void Material::DeInitialize()
{
	safeRelease<ID3D11VertexShader>(_pVertexShader);
	safeRelease<ID3D11PixelShader>(_pPixelShader);
	safeRelease<ID3D11InputLayout>(_pInputLayout);
}

wchar_t* ToWideSet(char* ptr)
{
	size_t size = strlen(ptr) + 1;
	wchar_t* res = new wchar_t[size];

	size_t outSize;
	mbstowcs_s(&outSize, res, size, ptr, size - 1);
	return res;
}

InitResult Material::CreateVertexShader(ID3D11Device* pD3DDevice, LPCWSTR name)
{
	ID3DBlob* pCompiledCode = nullptr;
	ID3DBlob* pCompileErrors = nullptr;

	std::wstring compiledName = std::wstring(name) + TEXT(".cso");

	HRESULT hr = D3DReadFileToBlob(compiledName.c_str(), &pCompiledCode);
	//MessageBox(NULL, std::to_wstring(hr).c_str(), name , 0);
	if (FAILED(hr))
	{
		std::wstring rawName = std::wstring(name) + TEXT(".hlsl");
		hr = D3DCompileFromFile(
			rawName.c_str(), // shader filename
			nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, // optional macros & includes
			"main", // entry point function name
			"vs_4_0", // shader type & version
			0, 0, // optional flags
			&pCompiledCode, // compiled code target
			&pCompileErrors // optional blob for all compile errors
		);
	}

	if (FAILED(hr))
	{
		std::wstringstream stream;
		stream << TEXT("Material: Failed to compile vertex shader.");

		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			stream << TEXT("File not found: ");
			stream << name;
		}

		if (pCompileErrors)
		{
			stream << TEXT("Compile Errors: ");
			stream << ToWideSet((char*)pCompileErrors->GetBufferPointer());
		}

		return InitResult::Failure(hr, stream.str().c_str());
	}

	hr = pD3DDevice->CreateVertexShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_pVertexShader);
	if (FAILED(hr)) return InitResult::Failure(hr, TEXT("Material: Failed to create vertex shader."));

	InitResult result = CreateInputLayout(pD3DDevice, pCompiledCode);
	if (result.Failed) return result;

	safeRelease<ID3DBlob>(pCompiledCode);

	return InitResult::Success();
}

InitResult Material::CreatePixelShader(ID3D11Device* pD3DDevice, LPCWSTR name)
{
	ID3DBlob* pCompiledCode = nullptr;
	ID3DBlob* pCompiledErrors = nullptr;
	std::wstring compiledName = std::wstring(name) + TEXT(".cso");

	HRESULT hr = D3DReadFileToBlob(compiledName.c_str(), &pCompiledCode);

	if (FAILED(hr))
	{
		std::wstring rawName = std::wstring(name) + TEXT(".hlsl");
		hr = D3DCompileFromFile(
			rawName.c_str(), // shader filename
			nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, // optional macros & includes
			"main", // entry point function name
			"ps_4_0", // shader type & version
			0, 0, // optional flags
			&pCompiledCode, // compiled code target
			&pCompiledErrors // optional blob for all compile errors
		);

	}
	if (FAILED(hr))
	{
		std::wstringstream stream;
		stream << TEXT("Material: Failed to compile pixel shader.");

		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			stream << TEXT("File not found: ");
			stream << name;
		}

		if (pCompiledErrors)
		{
			stream << TEXT("Compile Errors: ");
			stream << ToWideSet((char*)pCompiledErrors->GetBufferPointer());
		}

		return InitResult::Failure(hr, stream.str().c_str());
	}

	hr = pD3DDevice->CreatePixelShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_pPixelShader);
	if (FAILED(hr)) InitResult::Failure(hr, TEXT("Material: Failed to create pixel shader."));

	safeRelease<ID3DBlob>(pCompiledCode);

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
	if (FAILED(hr)) return InitResult::Failure(hr, TEXT("Material: Failed to create input layout."));

	return InitResult::Success();
}


