#pragma once
#ifndef DISTORTION_H
#define DISTORTION_H

#include "IDSP.h"
#include "../Math.h"
#include "../Debug.h"
#include "../Filters/LowPassFilter.h"
#include "../Filters/WaveShaperFilter.h"

class Distortion : public IDSP
{
public:
	Distortion(uint32 SampleRate)
		: m_LowPassFilter(SampleRate),
		  m_Rate(0)
	{
		SetRate(1);

		m_LowPassFilter.SetCutoffFrequency(500);

		static const WaveShaperFilter::TablePoints SHAPER_TABLE[] = {{-1, -1}, {-0.8, -0.2}, {-0.7, -0.5}, {-0.6, -0.2}, {-0.5, -0.5}, {0, 0}, {0.5, 0.5}, {0.6, 0.2}, {0.7, 0.5}, {0.8, 0.2}, {1, 1}};

		m_WaveShaperFilter.SetTable(SHAPER_TABLE, 11);
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
		float multiplier = (m_Rate + 1) * 40;

		for (uint16 i = 0; i < Count; ++i)
		{
			double input = m_LowPassFilter.Process(Buffer[i]);

			Buffer[i] = m_WaveShaperFilter.Process(input * multiplier) / multiplier;
		}
	}

private:
	LowPassFilter m_LowPassFilter;
	WaveShaperFilter m_WaveShaperFilter;
	float m_Rate;
};

#endif