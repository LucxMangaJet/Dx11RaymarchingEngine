#pragma once
#include "AppInfo.h"


enum class ShaderType
{
	VertexShader,
	PixelShader,
	ComputeShader
};

class ShaderUtility
{
public:
	static InitResult CompileShader(LPCWSTR shaderPath, ShaderType shaderType, ID3DBlob** compiledCode);
	static LPCSTR ShaderTypeToTarget(ShaderType type);

};
