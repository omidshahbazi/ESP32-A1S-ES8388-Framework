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
		SetBorderFrequencies(150, 5 * KHz, 5);
	}

	//[MIN_FREQUENCY, MAX_FREQUENCY]
	//[0, (LowMid / 10) - (MIN_FREQUENCY / 10)]
	void SetBorderFrequencies(float LowMid, float MidHigh, float Threshold = 5)
	{
		const float LowMidThreshold = Threshold * 10;
		const float MidHighThreshold = (Threshold * 0.1) * KHz;

		ASSERT(MIN_FREQUENCY <= LowMid && LowMid <= MAX_FREQUENCY, "Invalid LowMid");
		ASSERT(MIN_FREQUENCY <= MidHigh && MidHigh <= MAX_FREQUENCY, "Invalid MidHigh");
		ASSERT(MIN_FREQUENCY <= LowMid - LowMidThreshold, "Invalid Threshold");

		m_LowPassFilter.SetCutoffFrequency(LowMid);
		m_BandPassFilter.SetFrequencies(LowMid - LowMidThreshold, MidHigh);
		m_HighPassFilter.SetCutoffFrequency(MidHigh - MidHighThreshold);
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