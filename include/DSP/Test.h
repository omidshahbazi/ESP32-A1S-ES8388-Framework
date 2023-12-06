#pragma once
#ifndef TEST_H
#define TEST_H

#include "IDSP.h"
#include <math.h>
#include <stdio.h>
#include <sstream>

class Test : public IDSP
{
public:
	Test(uint32 SampleRate)
	{
	}

	void SetValue(float Value)
	{
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double input = Buffer[i];

			Buffer[i] = input / (1 + fabs(input));
		}
	}
};

#endif