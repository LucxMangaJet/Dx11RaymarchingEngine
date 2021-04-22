#pragma once
class Time
{
public:
	int init();
	void update();
	void deInit();

	float getDeltaTime() { return _deltaTime; }
	float getTotalTime() { return _totalTime; }

private:

	double _secondsPerCount;
	float _deltaTime;
	float _totalTime = 0.0f;
	__int64 _prevCount;
	__int64 _currCount;

};

