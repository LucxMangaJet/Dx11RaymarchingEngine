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
	//null check for key existence
	if (_shaderDict.find(STRING(shader)) == _shaderDict.end())
	{
		LOG_F(WARNING, "Failed to retrieve shader: %s", shader);
		return nullptr;
	}

	return 	_shaderDict[shader];
}

InitResult ShaderHandler::LoadAllShaders(bool recompileAll)
{
	_shaderDict.clear();

	if (recompileAll)
	LOG_F(INFO, "Compiling all shaders");
	else
	LOG_F(INFO, "Loading or compiling all shaders");

	for (int i = 0; i < _shaderInfoList.size(); i++)
	{
		ShaderInfo current = _shaderInfoList[i];
		ID3DBlob* compiledCode;

		WSTRING cachedPath = current.ShaderPath + TEXT(".cso");

		if (!recompileAll && ShaderUtility::DoesFileExist(cachedPath.c_str()))
		{
			//Load Cached
			LOG_F(INFO, "Found cached shader for %s, reading from file.", current.ShaderName.c_str());
			InitResult result = ShaderUtility::ReadFileToBlob(cachedPath.c_str(), &compiledCode);
			if (result.Failed) return result;
		}
		else
		{
			//Compile shader
			InitResult result = ShaderUtility::CompileShader(current.ShaderPath.c_str(), current.ShaderType, &compiledCode);
			if (result.Failed) return result;

			//Cache shader
			ShaderUtility::WriteToFile(compiledCode, cachedPath.c_str());
		}

		_shaderDict[current.ShaderName] = compiledCode;
	}
	LOG_F(INFO, "Finished loading shaders");

	return InitResult::Success();
}

void ShaderHandler::DeInitialize()
{
}
