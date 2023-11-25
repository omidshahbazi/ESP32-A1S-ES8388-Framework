#pragma once
#ifndef HIGH_PASS_FILTER_H
#define HIGH_PASS_FILTER_H

#include "Wave.h"
#include "../Math.h"

class HighPassFilter : public Wave
{
public:
	HighPassFilter(void)
		: m_CutoffFrequency(0),
		  m_Alpha(0),
		  m_PreviousInput(0),
		  m_PreviousOutput(0)
	{
		SetCutoffFrequency(1000);
	}

	void SetCutoffFrequency(float Value)
	{
		Value = Math::Clamp(Value, MIN_FREQUENCY, MAX_FREQUENCY);

		m_CutoffFrequency = Value;

		double timeConstant = 1 / (2 * Math::PI_VALUE * m_CutoffFrequency);

		m_Alpha = timeConstant / (timeConstant + DELTA_TIME);
	}
	float GetCutoffFrequency(void) const
	{
		return m_CutoffFrequency;
	}

	double Process(double Value) override
	{
		double output = (m_Alpha * m_PreviousOutput) + (m_Alpha * (Value - m_PreviousInput));

		m_PreviousInput = Value;
		m_PreviousOutput = output;

		return output;
	}

private:
	float m_CutoffFrequency;
	double m_Alpha;
	float m_PreviousInput;
	float m_PreviousOutput;

	static constexpr float DELTA_TIME = 1;
};

#endif