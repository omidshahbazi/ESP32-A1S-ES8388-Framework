#pragma once
#ifndef OSCILATOR_FILTER_H
#define OSCILATOR_FILTER_H

#include "Filter.h"
#include "../Math.h"
#include "../Debug.h"
#include <functional>

class OscillatorFilter : public Filter
{
public:
	typedef std::function<float(float)> OscillatorFunction;

public:
	OscillatorFilter(uint32 SampleRate)
		: m_SampleRate(SampleRate),
		  m_Freuency(0),
		  m_PhaseOffset(0),
		  m_DeltaPhase(0),
		  m_Phase(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetFunction(
			[](float value)
			{
				return sin(value);
			});

		SetFrequency(20);
	}

	void SetFunction(OscillatorFunction &&Function)
	{
		ASSERT(Function != nullptr, "Invalid Function");

		m_Function = Function;
	}

	//(0, MAX_FREQUENCY]
	void SetFrequency(float Value)
	{
		ASSERT(0 < Value && Value <= MAX_FREQUENCY, "Invalid Value");

		m_Freuency = Value;

		m_DeltaPhase = Math::TWO_PI_VALUE * m_Freuency / m_SampleRate;
	}
	float GetFrequency(void) const
	{
		return m_Freuency;
	}

	void SetPhaseOffset(float Value)
	{
		m_PhaseOffset = Value;
	}
	float GetPhaseOffset(void) const
	{
		return m_PhaseOffset;
	}

	double Process(void)
	{
		float value = m_Function(fmod(m_Phase + m_PhaseOffset, Math::TWO_PI_VALUE));

		m_Phase = fmod(m_Phase + m_DeltaPhase, Math::TWO_PI_VALUE);

		return value;
	}

private:
	double Process(double Value) override
	{
		return 0;
	}

private:
	uint32 m_SampleRate;
	float m_Freuency;
	float m_PhaseOffset;
	OscillatorFunction m_Function;

	float m_DeltaPhase;
	float m_Phase;
};

#endif