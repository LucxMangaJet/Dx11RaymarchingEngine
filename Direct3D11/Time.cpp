#include "Time.h"
#include <Windows.h>
#include <string>
#include <iomanip>
#include <sstream>

#pragma comment(lib, "Winmm.lib")

using namespace std;

int Time::init()
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	_secondsPerCount = 1.0 / (double)countsPerSec;

	__int64 currCont;
	QueryPerformanceCounter((LARGE_INTEGER*)&currCont);
	_prevCount = currCont;
	return 0;
}

void Time::update()
{
	__int64 currCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&currCount);
	_currCount = currCount;
	_deltaTime = (_currCount - _prevCount) * _secondsPerCount;

	_totalTime += _deltaTime;
	_prevCount = currCount;

#if _DEBUG
	std::wstringstream stream ;
	stream << TEXT("FPS : ");
	stream << std::fixed << std::setprecision(0) << (1.0f / _deltaTime);
	stream << TEXT(" DT: ");
	stream << std::setprecision(5) << _deltaTime;
	stream << std::endl;

	OutputDebugString(stream.str().c_str());
#endif
}

void Time::deInit()
{
}
