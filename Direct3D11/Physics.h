#pragma once
#include "AppInfo.h"


class Physics
{
public:
	const static int MAX_POINTS = 64;

	InitResult Initiate(const AppInfo& appInfo, LPCSTR shaderName);
	void Run(const AppInfo& appInfo);

	void Finish(const AppInfo& appInfo);

	void ResetFrame() { _currentID = 0; };
	POINT_ID SetPoint(V3 point);
	float GetDistance(POINT_ID id) { return _distances[id]; };

private:

	POINT_ID _currentID = 0;

	int _pointCount = 0;
	V3 _points[MAX_POINTS];
	float _distances[MAX_POINTS];

	ID3D11Buffer* _inputBuffer;
	ID3D11Buffer* _outputBuffer;
	ID3D11Buffer* _outputReadBuffer;

	ID3D11ShaderResourceView* _inputSRV;
	ID3D11UnorderedAccessView* _outputUAV;

	ID3D11ComputeShader* _computeShader;
	ID3D11Query* _finishedQuery;

};

