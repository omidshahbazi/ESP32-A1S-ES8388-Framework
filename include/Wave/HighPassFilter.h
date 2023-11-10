#pragma once
#ifndef HIGH_PASS_FILTER_H
#define HIGH_PASS_FILTER_H

#include "Wave.h"

#include "Wave.h"
#include "../Math.h"

class HighPassFilter : public Wave
{
public:
	HighPassFilter(void)
		: m_CutoffFrequency(0),
		  m_TimeConstant(0),
		  m_PreviousInput(0),
		  m_PreviousOutput(0)
	{
		SetCutoffFrequencye(1000);
	}

	void SetCutoffFrequencye(float Value)
	{
		m_CutoffFrequency = Value;
		m_TimeConstant = 1 / (2 * Math::PI * m_CutoffFrequency);
	}
	float GetCutoffFrequencye(void) const
	{
		return m_CutoffFrequency;
	}

	double Process(double Value) override
	{
		double alpha = m_TimeConstant / (1 + m_TimeConstant);

		double output = alpha * (m_PreviousOutput + Value - m_PreviousInput);

		m_PreviousInput = Value;
		m_PreviousOutput = output;

		return output;
	}

private:
	uint16 m_SampleRate;
	float m_CutoffFrequency;
	float m_TimeConstant;
	float m_PreviousInput;
	float m_PreviousOutput;
};

#endif