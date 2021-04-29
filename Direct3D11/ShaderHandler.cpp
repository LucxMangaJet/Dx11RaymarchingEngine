#include "ShaderHandler.h"
#include "json.hpp"
#include "loguru.hpp"


void ShaderHandler::AddShader(LPCSTR shaderName, LPCWSTR shaderPath, ShaderType type)
{

	ShaderInfo info;
	info.ShaderName = STRING(shaderName);
	info.ShaderPath = WSTRING(shaderPath);
	info.ShaderType = type;

	_shaderInfoList.push_back(info);

	LOG_F(INFO, "Added Shader %s to shader list", shaderName);
}

ID3DBlob* ShaderHandler::GetShader(LPCSTR shader)
{
	if (_shaderDict.find(STRING(shader)) == _shaderDict.end())
		return nullptr;

	return 	_shaderDict[shader];
}

InitResult ShaderHandler::CompileAllShaders()
{
	_shaderDict.clear();
	LOG_F(INFO, "Compiling all shaders");

	for (int i = 0; i <_shaderInfoList.size(); i++)
	{
		ShaderInfo current = _shaderInfoList[i];
		ID3DBlob* compiledCode;

		InitResult result = ShaderUtility::CompileShader(current.ShaderPath.c_str(), current.ShaderType, &compiledCode);
		if (result.Failed) return result;
	
		_shaderDict[current.ShaderName] = compiledCode;
	}
	LOG_F(INFO, "Finished compiling shaders");

	return InitResult::Success();
}

void ShaderHandler::DeInitialize()
{
}
