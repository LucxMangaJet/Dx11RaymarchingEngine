#pragma once
#include "AppInfo.h"
#include <chrono>
#include <string>
#include <comdef.h>  // string conversion

struct PerformanceTimer
{
	PerformanceTimer()
	{
		_start = std::chrono::high_resolution_clock::now();
	}

	double GetElapsed()
	{
		auto t_end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration<double, std::milli>(t_end - _start).count();
	}

	void LogElapsed(LPCSTR name)
	{
		double elapsed = GetElapsed();
		LOG_F(INFO, "PerformanceTimer: %s took %f ms", name, elapsed);
	}

	void LogElapsed(LPCWSTR name)
	{
		//https://stackoverflow.com/questions/12637779/how-to-convert-const-wchar-to-const-char/44375695
		_bstr_t b(name);
		LPCSTR c = b;
		double elapsed = GetElapsed();
		LOG_F(INFO, "PerformanceTimer: %s took %f ms", c, elapsed);
	}


private:
	std::chrono::steady_clock::time_point _start;

};
