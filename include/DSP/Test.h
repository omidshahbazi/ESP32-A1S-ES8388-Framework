#pragma once
#ifndef TEST__H
#define TEST__H

#include "IDSP.h"
#include "../Wave/WaveShaper.h"
#include <math.h>
#include <stdio.h>

class Test : public IDSP
{
public:
	Test(uint32 SampleRate)
	{
	}

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
	WaveShaper m_Wave;
};

#endif