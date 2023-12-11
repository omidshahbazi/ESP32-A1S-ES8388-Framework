#pragma once
#ifndef HIGH_PASS_FILTER_H
#define HIGH_PASS_FILTER_H

#include "Filter.h"
#include "../Math.h"
#include "../Debug.h"

class HighPassFilter : public Filter
{
public:
	HighPassFilter(uint32 SampleRate)
		: m_SampleRate(SampleRate),
		  m_DeltaTime(0),
		  m_CutoffFrequency(0),
		  m_Alpha(0),
		  m_CapacitorVoltage(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetDeltaTime(1);
		SetCutoffFrequency(MIN_FREQUENCY);
	}

	//(0, 10]
	void SetDeltaTime(float Value)
	{
		ASSERT(0 < Value && Value <= 10, "Invalid Value");

		m_DeltaTime = Value;

		Update();
	}
	float GetDeltaTime(void) const
	{
		return m_DeltaTime;
	}

	//[MIN_FREQUENCY, MAX_CUTOFF_FREQUENCY]
	void SetCutoffFrequency(float Value)
	{
		ASSERT(MIN_FREQUENCY <= Value && Value <= MAX_CUTOFF_FREQUENCY, "Invalid Value");

		m_CutoffFrequency = Value;

		Update();
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
	void Update(void)
	{
		double timeConstant = 1 / (Math::TWO_PI_VALUE * m_CutoffFrequency);

		m_Alpha = m_DeltaTime / (timeConstant * m_SampleRate);
	}

private:
	uint32 m_SampleRate;
	float m_DeltaTime;
	double m_CutoffFrequency;
	double m_Alpha;
	double m_CapacitorVoltage;

public:
	static constexpr double MAX_CUTOFF_FREQUENCY = 14066.730469;
};

#endif