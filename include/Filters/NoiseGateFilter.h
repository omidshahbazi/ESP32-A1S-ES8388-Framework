#pragma once
#ifndef NOISE_GATE_FILTER_H
#define NOISE_GATE_FILTER_H

#include "EnvelopeFollowerFilter.h"
#include "../Math.h"
#include "../Debug.h"

class NoiseGateFilter : private EnvelopeFollowerFilter
{
public:
	NoiseGateFilter(uint32 SampleRate)
		: EnvelopeFollowerFilter(SampleRate)
	{
		SetThreshold(0);

		EnvelopeFollowerFilter::SetAttackTime(0.02);
		EnvelopeFollowerFilter::SetReleaseTime(0.06);
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
			double gainFactor = 1 - (envelope / m_Threshold);
			// double gainFactor = Math::Clamp01(1.0 - ((envelope - m_Threshold) / (1 - m_Threshold)));

			Value *= gainFactor;
		}

		return Value;
	}

private:
	float m_Threshold;
};
#endif