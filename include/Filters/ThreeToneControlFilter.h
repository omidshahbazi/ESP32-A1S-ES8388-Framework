#pragma once
#ifndef THREE_TONE_CONTROL_FILTER_H
#define THREE_TONE_CONTROL_FILTER_H

#include "../Math.h"
#include "LowPassFilter.h"
#include "BandPassFilter.h"
#include "HighPassFilter.h"

// TODO: This doesn't work properly
class ThreeToneControlFilter : public Filter
{
public:
	ThreeToneControlFilter(uint32 SampleRate)
		: m_LowTone(0),
		  m_MidTone(0),
		  m_HighTone(0),
		  m_LowPassFilter(SampleRate),
		  m_BandPassFilter(SampleRate),
		  m_HighPassFilter(SampleRate)
	{
		m_LowPassFilter.SetCutoffFrequency(100);
		// m_LowPassFilter.SetResonance(6);

		m_BandPassFilter.SetFrequencies(100, 5 * KHz);
		m_BandPassFilter.SetResonance(6);

		m_HighPassFilter.SetCutoffFrequency(5 * KHz);
		// m_HighPassFilter.SetResonance(6);
	}

	//[-20dB, 0dB]
	void SetLowTone(float Value)
	{
		ASSERT(-20 <= Value && Value <= 0, "Invalid Value");

		m_LowTone = Value;
	}
	float GetLowTone(void) const
	{
		return m_LowTone;
	}

	//[-20dB, 0dB]
	void SetMidTone(float Value)
	{
		ASSERT(-20 <= Value && Value <= 0, "Invalid Value");

		m_MidTone = Value;
	}
	float GetMidTone(void) const
	{
		return m_MidTone;
	}

	//[-20dB, 0dB]
	void SetHighTone(float Value)
	{
		ASSERT(-20 <= Value && Value <= 0, "Invalid Value");

		m_HighTone = Value;
	}
	float GetHighTone(void) const
	{
		return m_HighTone;
	}

	double Process(double Value) override
	{
		return (m_LowPassFilter.Process(Value) * pow(10, m_LowTone / 20)) +
			   (m_BandPassFilter.Process(Value) * pow(10, m_MidTone / 20)) +
			   (m_HighPassFilter.Process(Value) * pow(10, m_HighTone / 20));

		// return (m_LowPassFilter.Process(Value) * pow(10, m_LowTone / 20)) +
		// 	   (m_BandPassFilter.Process(Value) * pow(10, m_MidTone / 20));

		// return m_HighPassFilter.Process(Value) * pow(10, m_HighTone / 20);
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