#pragma once
#ifndef NOISE_GATE_FILTER_H
#define NOISE_GATE_FILTER_H

#include "BandStopFilter.h"
#include "../Math.h"

class NoiseGateFilter : private BandStopFilter
{
public:
	NoiseGateFilter(uint32 SampleRate)
		: BandStopFilter(SampleRate),
		  m_LowerThreshold(0),
		  m_UpperThreshold(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetCenterFrequency(CUTOFF_FREQUENCY);
		SetCenterFrequency(1000);

		SetThreshold(0);
	}

	//[0, 1]
	void SetThreshold(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Threshold = Value;

		float dB = -70 + 60 * m_Threshold;

		m_UpperThreshold = powf(10, dB / 20);
		m_LowerThreshold = m_UpperThreshold / 2;
	}
	float GetThreshold(void)
	{
		return m_Threshold;
	}

	double Process(double Value) override
	{
		float envelope = 1.4142 * BandStopFilter::Process(fabsf(Value));

		// detecting the gate and expansion area
		if (envelope < m_LowerThreshold)
		{
			Value = 0;
		}
		else if (envelope < m_UpperThreshold)
		{
			Value *= (envelope - m_LowerThreshold) / (m_UpperThreshold - m_LowerThreshold);
		}

		return Value;
	}

private:
	float m_Threshold;
	double m_LowerThreshold;
	double m_UpperThreshold;

private:
	static constexpr float CUTOFF_FREQUENCY = 20;
};
#endif