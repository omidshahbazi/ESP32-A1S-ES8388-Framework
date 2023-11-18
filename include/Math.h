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

	// Ranges from 0 to 255
	template <typename T>
	static T TableLookupLinear(const T *Table, T Value)
	{
		uint32 index = (int32)Value;
		T frac = Value - (T)index;
		return Table[index] + (frac * (Table[index + 1] - Table[index]));
	}

public:
	static constexpr double PI_VALUE = 3.14159265;
};

#endif