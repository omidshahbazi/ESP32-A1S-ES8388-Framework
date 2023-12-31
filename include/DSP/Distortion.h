#pragma once
#ifndef DISTORTION_H
#define DISTORTION_H

#include "IDSP.h"
#include "../Math.h"
#include "../Debug.h"
#include "../Filters/LowPassFilter.h"

class Distortion : public IDSP
{
public:
	Distortion(uint32 SampleRate)
		: m_LowPassFilter(SampleRate),
		  m_Rate(0)
	{
		m_LowPassFilter.SetCutoffFrequency(2 * KHz);
		m_LowPassFilter.SetDeltaTime(0.001);
		SetRate(1);
	}

	//[0, 1]
	void SetRate(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Rate = Value;
	}
	float GetRate(void)
	{
		return m_Rate;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double input = m_LowPassFilter.Process(Buffer[i]);

			input = Math::ExponentialSaturation(input, (m_Rate + 1) * 1);

			Buffer[i] = Math::Clamp(input, -1, 1);
		}
	}

private:
	LowPassFilter m_LowPassFilter;
	float m_Rate;
};

#endif