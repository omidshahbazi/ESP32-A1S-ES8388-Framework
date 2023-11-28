#pragma once
#ifndef TEST_H
#define TEST_H

#include "IDSP.h"
#include "../Wave/NoiseGate.h"
#include <math.h>
#include <stdio.h>

class Test : public IDSP
{
public:
	void SetValue(float Value)
	{
		// m_Wave.SetThreshold(Value);
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		// for (uint16 i = 0; i < Count; ++i)
		// 	Buffer[i] -= fmod(Buffer[i], 0.1F * 1);

		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = m_Wave.Process(Buffer[i]);
	}

private:
	NoiseGate m_Wave;
};

#endif