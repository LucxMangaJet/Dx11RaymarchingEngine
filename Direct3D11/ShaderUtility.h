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
	static InitResult ReadFileToBlob(LPCWSTR path, ID3DBlob** blob);

	static void WriteToFile(ID3DBlob* data, LPCWSTR path);
	static bool DoesFileExist(LPCWSTR path);

	static int GetFileSize(LPCWSTR path);


	static LPCSTR ShaderTypeToTarget(ShaderType type);

};
