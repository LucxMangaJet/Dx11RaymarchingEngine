#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "D3D.h"
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

#define RMB_PER_RENDERING 0
#define  RMB_PER_MATERIAL 1
#define  RMB_PER_OBJECT 2


struct  LightData;

struct MaterialParameters
{
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT4 Reflect;
};

class Material
{
public:
	int init(ID3D11Device* d3dDevice, LPCWSTR textureName, LPCWSTR vertexShaderName, LPCWSTR pixelShaderName, MaterialParameters parameters);
	virtual void render(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix);
	void deInit();

protected:
	// helper methods
	int createVertexShader(ID3D11Device* pD3DDevice, LPCWSTR name);
	int createPixelShader(ID3D11Device* pD3DDevice, LPCWSTR name);
	int createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pBlob);

	// shaders
	ID3D11VertexShader* _pVertexShader = nullptr;
	ID3D11PixelShader* _pPixelShader = nullptr;

	// input layout
	ID3D11InputLayout* _pInputLayout = nullptr;
};
