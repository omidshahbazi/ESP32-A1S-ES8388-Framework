#pragma once
#ifndef Math_H
#define Math_H

#include <math.h>

class Math
{
public:
	template <typename T, typename U, typename V>
	static T Clamp(T Value, U Min, V Max)
	{
		if (Value < Min)
			return Min;

		if (Value > Max)
			return Max;

		return Value;
	}

	template <typename T>
	static T Clamp01(T Value)
	{
		return Clamp(Value, 0, 1);
	}

	template <typename T, typename U, typename V>
	static T Lerp(T Min, U Max, V Time)
	{
		Time = Clamp01(Time);

		T diff = Max - Min;

		return Min + (diff * Time);
	}

public:
	static const double PI_VALUE;
};

#endif