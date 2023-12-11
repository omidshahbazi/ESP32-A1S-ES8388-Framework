#pragma once
#ifndef THREE_TONE_CONTROL_FILTER_H
#define THREE_TONE_CONTROL_FILTER_H

#include "../Math.h"
#include "LowPassFilter.h"
#include "BandPassFilter.h"
#include "HighPassFilter.h"

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
		m_LowPassFilter.SetCenterFrequency(100);
		m_BandPassFilter.SetFrequencies(100, 5000);
		m_HighPassFilter.SetCenterFrequency(5000);
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
		return (m_LowRangeBandPassFilter.Process(Value) * pow(10, m_LowTone / 20)) +
			   (m_MidRangeBandPassFilter.Process(Value) * pow(10, m_MidTone / 20)) +
			   (m_HighRangeBandPassFilter.Process(Value) * pow(10, m_HighTone / 20));
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