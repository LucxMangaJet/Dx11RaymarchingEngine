#pragma once
#include "AppInfo.h"
#include "imgui.h"

#include <d3d11.h>


class ImGUIDirect3D
{
public:

	InitResult Initialize(const AppInfo& appInfo);

	void Update(const AppInfo& appInfo);
	void Render(const AppInfo& appInfo, ImDrawData* draw_data);

	void DeInitialize();

private:
	void SetupRenderState(ImDrawData* drawData, ID3D11DeviceContext* ctx);
	InitResult InitD3DObjects();
	void CreateFontsTexture();

private:
	ID3D11Device* _pd3dDevice = NULL;
	ID3D11DeviceContext* _pd3dDeviceContext = NULL;
	IDXGIFactory* _pFactory = NULL;
	ID3D11Buffer* _pVertexBuffer = NULL;
	ID3D11Buffer* _pIndexBuffer = NULL;
	ID3D11VertexShader* _pVertexShader = NULL;
	ID3D11InputLayout* _pInputLayout = NULL;
	ID3D11Buffer* _pVertexConstantBuffer = NULL;
	ID3D11PixelShader* _pPixelShader = NULL;
	ID3D11SamplerState* _pFontSampler = NULL;
	ID3D11ShaderResourceView* _pFontTextureView = NULL;
	ID3D11RasterizerState* _pRasterizerState = NULL;
	ID3D11BlendState* _pBlendState = NULL;
	ID3D11DepthStencilState* _pDepthStencilState = NULL;
	int _VertexBufferSize = 5000, _IndexBufferSize = 10000;

	struct VERTEX_CONSTANT_BUFFER
	{
		float   mvp[4][4];
	};
};

