#pragma once
#ifndef TEST_H
#define TEST_H

#include "IDSP.h"
#include "../Filters/BandPassFilter.h"
#include <math.h>
#include <stdio.h>
#include <sstream>

class Test : public IDSP
{
public:
	Test(uint32 SampleRate)
		: m_Wave()
	{
	}

	void SetValue(float Value)
	{
		m_Wave.SetCenterFrequency(Value * MAX_FREQUENCY);
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = m_Wave.Process(Buffer[i]);
	}

private:
	BandPassFilter m_Wave;
};

#endif