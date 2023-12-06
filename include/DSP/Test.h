#pragma once
#ifndef TEST_H
#define TEST_H

#include "IDSP.h"
#include "../Filters/ThreeToneControlFilter.h"
#include <math.h>
#include <stdio.h>
#include <sstream>

class Test : public IDSP
{
public:
	Test(uint32 SampleRate)
		: m_Wave(SampleRate)
	{
	}

	void SetValue(float Value)
	{
		m_Wave.SetMidTone(Value);
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = m_Wave.Process(Buffer[i]);
	}

private:
	ThreeToneControlFilter m_Wave;
};

#endif