#pragma once
#ifndef TEST__H
#define TEST__H

#include "IDSP.h"
#include "../Wave/LowPassFilter.h"
#include <math.h>
#include <stdio.h>

class Test : public IDSP
{
public:
	Test(uint32 SampleRate)
		: m_Wave(SampleRate)
	{
	}

	void SetValue(float Value)
	{
		m_Wave.SetCutoffFrequency(Math::Lerp(MIN_FREQUENCY, MAX_FREQUENCY, Value));
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		// for (uint16 i = 0; i < Count; ++i)
		// 	Buffer[i] -= fmod(Buffer[i], 0.1F * m_Value);

		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = m_Wave.Process(Buffer[i]);
	}

private:
	LowPassFilter m_Wave;
};

#endif