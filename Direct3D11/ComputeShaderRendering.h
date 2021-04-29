#pragma once
#include "AppInfo.h"
#include <d3d11.h>

class ComputeShaderRendering
{
public:
	InitResult Initiate(const AppInfo& appInfo, LPCWSTR shaderName);

	void Render(const AppInfo& appInfo);

private:

	InitResult InitiateOutputView(const AppInfo& appInfo);
	InitResult InitiateComputeShader(const AppInfo& appInfo, LPCWSTR shaderName);

	ID3D11ShaderResourceView* _outputSRV;
	ID3D11UnorderedAccessView* _outputUAV;
	ID3D11ComputeShader* _computeShader;
};

