#pragma once
#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#ifdef SIMPLE_LOW_HIGH_PASS_FILTER

#include "Filter.h"
#include "../Math.h"
#include "../Debug.h"

class LowPassFilter : public Filter
{
public:
	LowPassFilter(uint32 SampleRate)
		: m_SampleRate(SampleRate),
		  m_DeltaTime(0),
		  m_CutoffFrequency(0),
		  m_Alpha(0),
		  m_CapacitorVoltage(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetDeltaTime(1);
		SetCutoffFrequency(MAX_CUTOFF_FREQUENCY);
	}

	//(0, 10]
	void SetDeltaTime(float Value)
	{
		ASSERT(0 < Value && Value <= 1, "Invalid Value");

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

		return m_CapacitorVoltage;
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
	float m_CutoffFrequency;
	float m_Alpha;
	float m_CapacitorVoltage;

public:
	static constexpr double MAX_CUTOFF_FREQUENCY = 13278.734375;
};

#else

#include "BiquadFilter.h"
#include "../Math.h"
#include "../Debug.h"

class LowPassFilter : private BiquadFilter
{
public:
	LowPassFilter(uint32 SampleRate)
		: BiquadFilter(1),
		  m_SampleRate(SampleRate),
		  m_CutoffFrequency(1),
		  m_Resonance(1)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetCutoffFrequency(MAX_FREQUENCY);
		SetResonance(1);
	}

	//[MIN_FREQUENCY, MAX_FREQUENCY]
	void SetCutoffFrequency(float Value)
	{
		ASSERT(MIN_FREQUENCY <= Value && Value <= MAX_FREQUENCY, "Invalid Value");

		m_CutoffFrequency = Value;

		Update();
	}
	float GetCutoffFrequency(void) const
	{
		return m_CutoffFrequency;
	}

	//(0, 4000]
	void SetResonance(float Value)
	{
		ASSERT(0 < Value && Value <= 4000, "Invalid Value");

		m_Resonance = Value;

		Update();
	}
	float GetResonance(void) const
	{
		return m_Resonance;
	}

	double Process(double Value) override
	{
		return BiquadFilter::Process(Value);
	}

private:
	void Update(void)
	{
		BiquadFilter::SetLowPassFilterCoefficients(this, m_SampleRate, m_CutoffFrequency, m_Resonance);
	}

private:
	uint32 m_SampleRate;
	float m_CutoffFrequency;
	float m_Resonance;
};

#endif
#endif