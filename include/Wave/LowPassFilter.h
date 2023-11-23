#pragma once
#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#include "Wave.h"
#include "../Math.h"

// https://en.wikipedia.org/wiki/Low-pass_filter
class LowPassFilter : public Wave
{
public:
	LowPassFilter(void)
		: m_CutoffFrequency(0),
		  m_Alpha(0),
		  m_PreviousOutput(0)
	{
		SetCutoffFrequency(1000);
	}

	void SetCutoffFrequency(float Value)
	{
		Value = Math::Clamp(Value, MIN_FREQUENCY, MAX_FREQUENCY);

		m_CutoffFrequency = Value;

		double timeConstant = 1 / (2 * Math::PI_VALUE * m_CutoffFrequency);

		m_Alpha = DELTA_TIME / (timeConstant + DELTA_TIME);
	}
	float GetCutoffFrequency(void) const
	{
		return m_CutoffFrequency;
	}

	double Process(double Value) override
	{
		double output = (m_Alpha * Value) + ((1 - m_Alpha) * m_PreviousOutput);

		m_PreviousOutput = output;

		return output;
	}

private:
	float m_CutoffFrequency;
	double m_Alpha;
	float m_PreviousOutput;

	static constexpr float DELTA_TIME = 1;
};

#endif