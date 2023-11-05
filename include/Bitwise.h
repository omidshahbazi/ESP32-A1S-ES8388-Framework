#pragma once
#ifndef BITWISE_H
#define BITWISE_H

#include "Common.h"

class Bitwise
{
public:
	template <typename T, typename U>
	static bool IsEnabled(T Mask, U Bits)
	{
		return (((int32)Mask & (int32)Bits) == (int32)Bits);
	}
};

template <typename T, typename U>
T operator|(T Left, U Right)
{
	return (T)((int32)Left | (int32)Right);
}

template <typename T, typename U>
T &operator|=(T &Left, U Right)
{
	Left = (T)((int32)Left | (int32)Right);
	return Left;
}

#endif