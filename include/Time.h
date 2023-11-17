#pragma once
#ifndef TIME_H
#define TIME_H

#include <chrono>

using namespace std::chrono;
using namespace std::chrono::_V2;

class Time
{
public:
	static void Initialize(void)
	{
		GetBeginning();
	}

	static double Now(void)
	{
		high_resolution_clock::time_point now = high_resolution_clock::now();

		return duration_cast<milliseconds>(now - GetBeginning()).count() / 1000.0F;
	}

private:
	static const high_resolution_clock::time_point &GetBeginning(void)
	{
		static high_resolution_clock::time_point beginning = high_resolution_clock::now();

		return beginning;
	}
};

#endif