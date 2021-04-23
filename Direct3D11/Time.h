#pragma once
#include "AppInfo.h"

class Time
{
public:
	InitResult Initialize();
	void Update();
	void DeInitialize();

	inline float GetDeltaTime() { return _deltaTime; }
	inline float GetTotalTime() { return _totalTime; }

private:

	double _secondsPerCount;
	float _deltaTime;
	float _totalTime = 0.0f;
	__int64 _prevCount;
	__int64 _currCount;

};

