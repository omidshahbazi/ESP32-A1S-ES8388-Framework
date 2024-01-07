#pragma once
#ifndef THREE_TONE_CONTROL_FILTER_H
#define THREE_TONE_CONTROL_FILTER_H

#include "../Math.h"
#include "LowPassFilter.h"
#include "BandPassFilter.h"
#include "HighPassFilter.h"

class ThreeToneControlFilter : public Filter
{
#define MULTIPLIER 0.0495

public:
	ThreeToneControlFilter(uint32 SampleRate)
		: m_LowTone(0),
		  m_MidTone(0),
		  m_HighTone(0),
		  m_LowPassFilter(SampleRate),
		  m_BandPassFilter(SampleRate),
		  m_HighPassFilter(SampleRate)
	{
		m_LowPassFilter.SetCutoffFrequency(150);
		m_BandPassFilter.SetFrequencies(100, 5 * KHz);
		m_HighPassFilter.SetCutoffFrequency(4.5 * KHz);
	}

	//[-20dB, 20dB]
	void SetLowTone(float Value)
	{
		ASSERT(-20 <= Value && Value <= 20, "Invalid Value");

		m_LowTone = Value;
	}
	float GetLowTone(void) const
	{
		return m_LowTone;
	}

	//[-20dB, 20dB]
	void SetMidTone(float Value)
	{
		ASSERT(-20 <= Value && Value <= 20, "Invalid Value");

		m_MidTone = Value;
	}
	float GetMidTone(void) const
	{
		return m_MidTone;
	}

	//[-20dB, 20dB]
	void SetHighTone(float Value)
	{
		ASSERT(-20 <= Value && Value <= 20, "Invalid Value");

		m_HighTone = Value;
	}
	float GetHighTone(void) const
	{
		return m_HighTone;
	}

	double Process(double Value) override
	{
		return (m_LowPassFilter.Process(Value) * (1 + (m_LowTone * MULTIPLIER))) +
			   (m_BandPassFilter.Process(Value) * (1 + (m_MidTone * MULTIPLIER))) +
			   (m_HighPassFilter.Process(Value) * (1 + (m_HighTone * MULTIPLIER)));
	}

private:
	float m_LowTone;
	float m_MidTone;
	float m_HighTone;

	LowPassFilter m_LowPassFilter;
	BandPassFilter m_BandPassFilter;
	HighPassFilter m_HighPassFilter;
};
#endif