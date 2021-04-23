#include "Time.h"
#include <Windows.h>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

InitResult Time::Initialize()
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	_secondsPerCount = 1.0 / (double)countsPerSec;

	__int64 currCont;
	QueryPerformanceCounter((LARGE_INTEGER*)&currCont);
	_prevCount = currCont;
	return InitResult::Success();
}

void Time::Update()
{
	__int64 currCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&currCount);
	_currCount = currCount;
	_deltaTime = (_currCount - _prevCount) * _secondsPerCount;

	_totalTime += _deltaTime;
	_prevCount = currCount;
}

void Time::DeInitialize()
{
}
