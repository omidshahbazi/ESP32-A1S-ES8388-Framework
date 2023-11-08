#pragma once
#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#include "Wave.h"
#include "../Math.h"

class LowPassFilter : public Wave
{
public:
	LowPassFilter(void)
		: m_CutoffFrequency(0),
		  m_TimeConstant(0),
		  m_PreviousOutput(0)
	{
		SetCutoffFrequencye(1000);
	}

	void SetCutoffFrequencye(float Value)
	{
		m_TimeConstant = 1 / (2 * Math::PI * Value);
	}
	float GetCutoffFrequencye(void) const
	{
		return m_CutoffFrequency;
	}

	float Process(float Value) override
	{
		double alpha = 1 / (1 + m_TimeConstant);

		double output = alpha * Value + (1 - alpha) * m_PreviousOutput;

		m_PreviousOutput = output;

		return output;
	}

private:
	float m_CutoffFrequency;
	float m_TimeConstant;
	float m_PreviousOutput;
};

#endif