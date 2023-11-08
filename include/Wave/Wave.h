#pragma once
#ifndef WAVE_H
#define WAVE_H

#include "../Common.h"

class Wave
{
public:
	virtual float Process(float Value) = 0;

	virtual void ProcessBuffer(float *Buffer, uint16 Count)
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = Process(Buffer[i]);
	}
};

#endif