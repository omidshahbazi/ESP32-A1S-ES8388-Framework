#pragma once
#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#include "Wave.h"
#include "../Math.h"
#include <stdio.h>

class LowPassFilter : public Wave
{
public:
	LowPassFilter(uint32 SampleRate)
		: m_SampleRate(0),
		  m_CutoffFrequency(0),
		  m_Alpha(0),
		  m_PreviousOutput(0)
	{
		m_SampleRate = Math::Clamp(SampleRate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);

		SetCutoffFrequency(1000);
	}

	void SetCutoffFrequency(float Value)
	{
		Value = Math::Clamp(Value, MIN_FREQUENCY, MAX_FREQUENCY);

		m_CutoffFrequency = Value;

		double timeConstant = 1 / (2 * Math::PI_VALUE * m_CutoffFrequency);

		m_Alpha = 1 / (timeConstant / m_SampleRate);

		// printf("Vals %f, %f, %f", Value, timeConstant, m_Alpha);
	}
	float GetCutoffFrequency(void) const
	{
		return m_CutoffFrequency;
	}

	double Process(double Value) override
	{
		float delta = (Value - m_PreviousOutput) * m_Alpha;

		m_PreviousOutput += delta;

		return m_PreviousOutput;
	}

private:
	uint32 m_SampleRate;
	float m_CutoffFrequency;
	double m_Alpha;
	float m_PreviousOutput;
};

#endif