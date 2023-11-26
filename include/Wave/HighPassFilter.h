#pragma once
#ifndef HIGH_PASS_FILTER_H
#define HIGH_PASS_FILTER_H

#include "Wave.h"
#include "../Math.h"

class HighPassFilter : public Wave
{
public:
	HighPassFilter(uint32 SampleRate)
		: m_SampleRate(0),
		  m_CutoffFrequency(0),
		  m_Alpha(0),
		  m_CapacitorVoltage(0)
	{
		m_SampleRate = Math::Clamp(SampleRate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);

		SetCutoffFrequency(MIN_FREQUENCY);
	}

	void SetCutoffFrequency(float Value)
	{
		Value = Math::Clamp(Value, MIN_FREQUENCY, MAX_CUTOFF_FREQUENCY);

		m_CutoffFrequency = Value;

		double timeConstant = 1 / (2 * Math::PI_VALUE * m_CutoffFrequency);

		m_Alpha = 1 / (timeConstant * m_SampleRate);
	}
	float GetCutoffFrequency(void) const
	{
		return m_CutoffFrequency;
	}

	double Process(double Value) override
	{
		double delta = (Value - m_CapacitorVoltage) * m_Alpha;

		m_CapacitorVoltage += delta;

		return Value - m_CapacitorVoltage;
	}

private:
	uint32 m_SampleRate;
	double m_CutoffFrequency;
	double m_Alpha;
	double m_CapacitorVoltage;

	static constexpr double MAX_CUTOFF_FREQUENCY = 14066.730469;
};

#endif