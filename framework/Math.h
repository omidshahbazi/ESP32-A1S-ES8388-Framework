#pragma once
#ifndef Math_H
#define Math_H

#include "Debug.h"

class Math
{
public:
	template <typename T, typename U, typename V>
	static T Clamp(T Value, U Min, V Max)
	{
		ASSERT(Min <= Max, TAG, "Min value %f cannot be bigger than Max value %f", Min, Max);

		if (Value < Min)
			return Min;

		if (Value > Max)
			return Max;

		return Value;
	}

private:
	static const char *TAG;
};

const char *Math::TAG = "Math";

#endif