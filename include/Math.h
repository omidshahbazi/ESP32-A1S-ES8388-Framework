#pragma once
#ifndef Math_H
#define Math_H

#include <cmath>
#include <type_traits>

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

	template <typename T>
	static T ClampExcluded0To1(T Value)
	{
		return Clamp(Value, EPSILON, 1);
	}

	template <typename T, typename U, typename V>
	static T Lerp(T Min, U Max, V Time)
	{
		static_assert(std::is_same<T, float>() || std::is_same<T, double>(), "T must be float or double");

		Time = Clamp01(Time);

		return (Min * (1 - Time)) + (Max * Time);
	}

	template <typename T, typename U>
	static T Min(T A, U B)
	{
		return (A < B ? A : B);
	}

	template <typename T, typename U>
	static T Max(T A, U B)
	{
		return (A > B ? A : B);
	}

	template <typename T>
	static int8 Sign(T Value)
	{
		return (0 < Value) - (Value < 0);
	}

	template <typename T>
	static T SoftClip(T Value, float Factor)
	{
		static_assert(std::is_same<T, float>() || std::is_same<T, double>(), "T must be float or double");

		return atan(Value) * Factor;
	}

	// Factor [0, 1]
	template <typename T>
	static T SymmetricalSoftClip(T Value, float Factor)
	{
		static_assert(std::is_same<T, float>() || std::is_same<T, double>(), "T must be float or double");

		T absValue = abs(Value);

		if (absValue < Factor)
			return 2 * Value;

		if (Factor <= absValue && absValue < Factor * 2)
			return ((3 - pow(2 - (3 * absValue), 2)) / 3) * Sign(Value);

		// if (Factor * 2 <= absValue)
		return Sign(Value);
	}

	template <typename T>
	static T HardClip(T Value, float Factor)
	{
		static_assert(std::is_same<T, float>() || std::is_same<T, double>(), "T must be float or double");

		return (abs(Value) > Factor ? Sign(Value) : Value);
	}

	template <typename T>
	static T ExponentialDiodClip(T Value)
	{
		static_assert(std::is_same<T, float>() || std::is_same<T, double>(), "T must be float or double");

		if (Value < 0)
			Value = -1 + exp(Value);
		else if (Value > 0)
			Value = 1 - exp(-Value);
		else
			Value = 0;

		return Value;
	}

	template <typename T>
	static T ExponentialSaturation(T Value, float Factor)
	{
		static_assert(std::is_same<T, float>() || std::is_same<T, double>(), "T must be float or double");

		// return pow(abs(Value), 2) * Math::Sign(Value) * Factor;

		return exp(abs(Value)) * Math::Sign(Value) * 0.37 * Factor;
	}

	template <typename T>
	static T AsymmetricSineSaturation(T Value, float Factor)
	{
		static_assert(std::is_same<T, float>() || std::is_same<T, double>(), "T must be float or double");

		return sin(Value * HALF_PI_VALUE) * Factor;
	}

	template <typename T>
	static T FoldbackDistortion(T Value, float Factor)
	{
		static_assert(std::is_same<T, float>() || std::is_same<T, double>(), "T must be float or double");

		return (abs(Value) > Factor ? Value : -1);
	}

public:
	static constexpr double PI_VALUE = 3.14159265;
	static constexpr double HALF_PI_VALUE = PI_VALUE / 2;
	static constexpr double TWO_PI_VALUE = 2 * PI_VALUE;
	static constexpr double EPSILON = 0.0001F;
};

#endif