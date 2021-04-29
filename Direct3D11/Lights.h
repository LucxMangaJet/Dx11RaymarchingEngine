#pragma once
#include "PerRenderingDataContainer.h"

class Lights
{

public:
	LightData* GetLightData() { return &_lightData; }
	void SetLightData(LightData data) { _lightData = data; }

private:
	LightData _lightData;
};

