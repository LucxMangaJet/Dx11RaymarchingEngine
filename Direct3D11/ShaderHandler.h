#pragma once
#include <vector>
#include <string>
#include "ShaderUtility.h"
#include <unordered_map>

class ShaderHandler
{

	struct ShaderInfo
	{
		STRING ShaderName;
		WSTRING ShaderPath;
		ShaderType ShaderType;
	};

public:

	void AddShader(LPCSTR shaderName, LPCWSTR shaderPath, ShaderType type);
	ID3DBlob* GetShader(LPCSTR shader);

	InitResult LoadAllShaders(bool recompileAll = false);

	void DeInitialize();
	 
private:

	std::unordered_map <STRING, ID3DBlob*> _shaderDict;
	std::vector<ShaderInfo> _shaderInfoList;
};

