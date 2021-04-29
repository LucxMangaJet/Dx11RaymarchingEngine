#include "ShaderUtility.h"
#include <d3dcompiler.h>
#include "PerformanceTimer.h"

InitResult ShaderUtility::CompileShader(LPCWSTR shaderPath, ShaderType shaderType, ID3DBlob** compiledCode)
{
	ID3DBlob* pCompileErrors = nullptr;
	LPCSTR target = ShaderUtility::ShaderTypeToTarget(shaderType);

#ifdef _DEBUG
	PerformanceTimer timer;
#endif // _DEBUG

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
		std::stringstream stream;
		stream << "Material: Failed to compile shader.";

		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			stream << "File not found: ";
			stream << shaderPath;
		}

		if (pCompileErrors)
		{
			stream << "Compile Errors: ";
			stream << (char*)pCompileErrors->GetBufferPointer();
		}

		return InitResult::Failure(hr, stream.str().c_str());
	}

#ifdef _DEBUG
	timer.LogElapsed(shaderPath);
#endif // DEBUG


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
