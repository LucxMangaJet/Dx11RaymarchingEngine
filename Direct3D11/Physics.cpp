#include "Physics.h"
#include <d3d11.h>
#include "ShaderHandler.h"
#include "PerformanceTimer.h"

InitResult Physics::Initiate(const AppInfo& appInfo, LPCSTR shaderName)
{
	//input buffer
	D3D11_BUFFER_DESC inputDesc;
	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.ByteWidth = sizeof(V3) * MAX_POINTS;
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof(V3);
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_SUBRESOURCE_DATA vinitDataA;
	vinitDataA.pSysMem = _points;

	HRESULT res = appInfo.D3DDevice->CreateBuffer(&inputDesc, &vinitDataA, &_inputBuffer);
	if (FAILED(res)) return InitResult::Failure(res, "Failed to create physics compute shader input buffer.");

	//output buffer
	D3D11_BUFFER_DESC outputDesc;
	outputDesc.Usage = D3D11_USAGE_DEFAULT;
	outputDesc.ByteWidth = sizeof(float) * MAX_POINTS;
	outputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	outputDesc.CPUAccessFlags = 0;
	outputDesc.StructureByteStride = sizeof(float);
	outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	res = appInfo.D3DDevice->CreateBuffer(&outputDesc, NULL, &_outputBuffer);
	if (FAILED(res)) return InitResult::Failure(res, "Failed to create physics compute shader output buffer.");


	//input view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = MAX_POINTS;

	res = appInfo.D3DDevice->CreateShaderResourceView(_inputBuffer, &srvDesc, &_inputSRV);
	if (FAILED(res)) return InitResult::Failure(res, "Failed to create physics compute shader input buffer SRV.");

	//output UAV
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = MAX_POINTS;

	res = appInfo.D3DDevice->CreateUnorderedAccessView(_outputBuffer, &uavDesc, &_outputUAV);
	if (FAILED(res)) return InitResult::Failure(res, "Failed to create physics compute shader output buffer UAV.");


	//create compute shader;
	ID3DBlob* pCompiledCode = appInfo.ShaderHander->GetShader(shaderName);
	res = appInfo.D3DDevice->CreateComputeShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_computeShader);
	if (FAILED(res)) return InitResult::Failure(res, "Failed to create physics compute shader.");

	//Create output read buffer
	D3D11_BUFFER_DESC outputReadDesc;
	outputReadDesc.Usage = D3D11_USAGE_STAGING;
	outputReadDesc.BindFlags = 0;
	outputReadDesc.ByteWidth = sizeof(float) * MAX_POINTS;
	outputReadDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	outputReadDesc.StructureByteStride = sizeof(float);
	outputReadDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	res = appInfo.D3DDevice->CreateBuffer(&outputReadDesc, 0, &_outputReadBuffer);
	if (FAILED(res)) return InitResult::Failure(res, "Failed to create physics compute shader output read buffer.");

	//finish query
	D3D11_QUERY_DESC queryDesc;
	queryDesc.Query = D3D11_QUERY_EVENT;
	queryDesc.MiscFlags = 0;

	res = appInfo.D3DDevice->CreateQuery(&queryDesc, &_finishedQuery);
	if (FAILED(res)) return InitResult::Failure(res, "Failed to create physics DX11 query");

	return InitResult::Success();
}

void Physics::Run(const AppInfo& appInfo)
{
	ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
	ID3D11ShaderResourceView* ppSRViewNULL[1] = { NULL };

	appInfo.D3DDeviceContext->Begin(_finishedQuery);

	appInfo.D3DDeviceContext->CSSetShader(_computeShader, NULL, 0);
	appInfo.D3DDeviceContext->CSSetUnorderedAccessViews(0, 1, &_outputUAV, NULL);
	appInfo.D3DDeviceContext->CSSetShaderResources(0, 1, &_inputSRV);

	//HOW MANY? 16*4 = 64 = MAX_POINTS
	appInfo.D3DDeviceContext->Dispatch(4, 1, 1);

	//cleanup
	appInfo.D3DDeviceContext->CSSetShader(NULL, NULL, 0);
	appInfo.D3DDeviceContext->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, NULL);
	appInfo.D3DDeviceContext->CSSetShaderResources(0, 1, ppSRViewNULL);

	appInfo.D3DDeviceContext->End(_finishedQuery);

	//Is flushing necessary?
	appInfo.D3DDeviceContext->Flush();

	Finish(appInfo);
}

void Physics::Finish(const AppInfo& appInfo)
{

	// Copy the output buffer to system memory.
	appInfo.D3DDeviceContext->CopyResource(_outputReadBuffer, _outputBuffer);
	// Map the data for reading.
	D3D11_MAPPED_SUBRESOURCE mappedData;
	appInfo.D3DDeviceContext->Map(_outputReadBuffer, 0, D3D11_MAP_READ, 0, &mappedData);
	float* dataView = reinterpret_cast<float*>(mappedData.pData);

	for (int i = 0; i < MAX_POINTS; i++)
	{
		_distances[i] = dataView[i];
	}

	LOG_F(INFO, "Physics Result: %f", _distances[0]);

	appInfo.D3DDeviceContext->Unmap(_outputReadBuffer, 0);
}
