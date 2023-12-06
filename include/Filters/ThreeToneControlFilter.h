#pragma once
#ifndef THREE_TONE_CONTROL_FILTER_H
#define THREE_TONE_CONTROL_FILTER_H

#include "../Math.h"
#include "../Debug.h"
#include "BandPassFilter.h"

class ThreeToneControlFilter : public Filter
{
public:
	ThreeToneControlFilter(uint32 SampleRate)
		: m_LowTone(1),
		  m_MidTone(1),
		  m_HighTone(1),
		  m_LowRangeBandPassFilter(SampleRate),
		  m_MidRangeBandPassFilter(SampleRate),
		  m_HighRangeBandPassFilter(SampleRate)
	{
		m_LowRangeBandPassFilter.SetCenterFrequency(150);
		m_LowRangeBandPassFilter.SetBandwidth(300);

		m_MidRangeBandPassFilter.SetCenterFrequency(1850);
		m_MidRangeBandPassFilter.SetBandwidth(3700);

		m_HighRangeBandPassFilter.SetCenterFrequency(8077.25);
		m_HighRangeBandPassFilter.SetBandwidth(16154.5);
	}

	//[0, 1]
	void SetLowTone(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_LowTone = Value;
	}
	float GetLowTone(void) const
	{
		return m_LowTone;
	}

	//[0, 1]
	void SetMidTone(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_MidTone = Value;
	}
	float GetMidTone(void) const
	{
		return m_MidTone;
	}

	//[0, 1]
	void SetHighTone(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_HighTone = Value;
	}
	float GetHighTone(void) const
	{
		return m_HighTone;
	}

	double Process(double Value) override
	{
		return Math::Lerp(0.0, m_LowRangeBandPassFilter.Process(Value), m_LowTone) +
			   Math::Lerp(0.0, m_MidRangeBandPassFilter.Process(Value), m_MidTone) +
			   Math::Lerp(0.0, m_HighRangeBandPassFilter.Process(Value), m_HighTone);
	}

private:
	float m_LowTone;
	float m_MidTone;
	float m_HighTone;

	BandPassFilter m_LowRangeBandPassFilter;
	BandPassFilter m_MidRangeBandPassFilter;
	BandPassFilter m_HighRangeBandPassFilter;
};
#endif