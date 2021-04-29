#include "ComputeShaderRendering.h"
#include <d3dcompiler.h>
#include "ShaderUtility.h"


InitResult ComputeShaderRendering::Initiate(const AppInfo& appInfo, LPCWSTR shaderName)
{
	InitResult result;

	result = InitiateOutputView(appInfo);
	if (result.Failed) return result;

	result = InitiateComputeShader(appInfo, shaderName);
	if (result.Failed) return result;

	return InitResult();
}

void ComputeShaderRendering::Render(const AppInfo& appInfo)
{
	appInfo.D3DDeviceContext->CSSetShader(_computeShader, NULL, 0);
	appInfo.D3DDeviceContext->CSSetUnorderedAccessViews(0, 1, &_outputUAV, NULL);
	//Set buffers?
	
	appInfo.D3DDeviceContext->Dispatch(appInfo.Width, appInfo.Height, 1);

	appInfo.D3DDeviceContext->CSSetShader(NULL, NULL, 0);
	appInfo.D3DDeviceContext->CSSetUnorderedAccessViews(0, 1, NULL, NULL);
	//Unset buffers?
}

InitResult ComputeShaderRendering::InitiateOutputView(const AppInfo& appInfo)
{
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM; //???

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = appInfo.Width;
	texDesc.Height = appInfo.Height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = format;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;


	ID3D11Texture2D* texture = 0;
	HRESULT result = appInfo.D3DDevice->CreateTexture2D(&texDesc, NULL, &texture);
	if (FAILED(result)) return InitResult::Failure(result, "Failed to create Compute shader texture.");

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView* outputTextureSRV;
	result = appInfo.D3DDevice->CreateShaderResourceView(texture, &srvDesc, &outputTextureSRV);
	if (FAILED(result)) return InitResult::Failure(result, "Failed to create compute shader resource view.");
	_outputSRV = outputTextureSRV;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	ID3D11UnorderedAccessView* outputTextureUAV;
	result = appInfo.D3DDevice->CreateUnorderedAccessView(texture, &uavDesc, &outputTextureUAV);
	if (FAILED(result)) return InitResult::Failure(result, "Failed to create compute shader unordered access view.");
	_outputUAV = outputTextureUAV;

	SafeRelease(texture);

	return InitResult::Success();
}

InitResult ComputeShaderRendering::InitiateComputeShader(const AppInfo& appInfo, LPCWSTR shaderName)
{
	ID3DBlob* compiledCode;

	InitResult result = ShaderUtility::CompileShader(shaderName, ShaderType::ComputeShader, &compiledCode);
	if (result.Failed) return result;

	HRESULT hres = appInfo.D3DDevice->CreateComputeShader(compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), nullptr, &_computeShader);
	if (FAILED(hres)) return InitResult::Failure(hres, "Failed to create compute shader.");



	return InitResult::Success();
}
