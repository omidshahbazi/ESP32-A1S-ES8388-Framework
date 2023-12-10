#pragma once
#ifndef NOISE_GATE_FILTER_H
#define NOISE_GATE_FILTER_H

#include "EnvelopeFollowerFilter.h"
#include "../Math.h"

class NoiseGateFilter : private EnvelopeFollowerFilter
{
public:
	NoiseGateFilter(uint32 SampleRate)
		: EnvelopeFollowerFilter(SampleRate, 0.005, 0.02)
	{
		SetThreshold(0);
	}

	//[0, 1]
	void SetThreshold(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Threshold = Value;
	}

	float GetThreshold(void) const
	{
		return m_Threshold;
	}

	double Process(double Value) override
	{
		double envelope = EnvelopeFollowerFilter::Process(Value);

		if (envelope < m_Threshold)
		{
			// Calculate a gain factor based on the envelope level
			double gainFactor = 1 - (envelope / m_Threshold);

			Value *= gainFactor;
		}

		return Value;
	}

private:
	float m_Threshold;
};
#endif