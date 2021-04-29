#include "ShaderUtility.h"
#include <d3dcompiler.h>

wchar_t* ToWideSet(char* ptr)
{
	size_t size = strlen(ptr) + 1;
	wchar_t* res = new wchar_t[size];

	size_t outSize;
	mbstowcs_s(&outSize, res, size, ptr, size - 1);
	return res;
}

InitResult ShaderUtility::CompileShader(LPCWSTR shaderPath, ShaderType shaderType, ID3DBlob** compiledCode)
{
	ID3DBlob* pCompileErrors = nullptr;
	LPCSTR target = ShaderUtility::ShaderTypeToTarget(shaderType);

	HRESULT hr = D3DCompileFromFile(
		shaderPath, // shader filename
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, // optional macros & includes
		"main", // entry point function name
		target, // shader type & version
		0, 0, // optional flags
		compiledCode, // compiled code target
		&pCompileErrors // optional blob for all compile errors
	);

	if (FAILED(hr))
	{
		std::wstringstream stream;
		stream << TEXT("Material: Failed to compile vertex shader.");

		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			stream << TEXT("File not found: ");
			stream << shaderPath;
		}

		if (pCompileErrors)
		{
			stream << TEXT("Compile Errors: ");
			stream << ToWideSet((char*)pCompileErrors->GetBufferPointer());
		}

		return InitResult::Failure(hr, stream.str().c_str());
	}

	SafeRelease(pCompileErrors);
    return InitResult::Success();
}

LPCSTR ShaderUtility::ShaderTypeToTarget(ShaderType type)
{
	switch (type)
	{
	case ShaderType::VertexShader:
		return "vs_4_0";
		break;
	case ShaderType::PixelShader:
		return "ps_4_0";
		break;
	case ShaderType::ComputeShader:
		return "cs_5_0";
		break;
	default:
		return "ERROR: Unsupported shader.";
		break;
	}

}
