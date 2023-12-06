#pragma once
#ifndef BAND_STOP_FILTER_H
#define BAND_STOP_FILTER_H

#include "BiquadFilter.h"
#include "../Math.h"

class BandStopFilter : private BiquadFilter
{
public:
	BandStopFilter(uint32 SampleRate)
		: BiquadFilter(1),
		  m_SampleRate(SampleRate),
		  m_CenterFrequency(1),
		  m_Bandwidth(1)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetBandwidth(2000);
		SetCenterFrequency(100);
	}

	//[MIN_FREQUENCY, MAX_FREQUENCY]
	void SetCenterFrequency(float Value)
	{
		ASSERT(MIN_FREQUENCY <= Value && Value <= MAX_FREQUENCY, "Invalid Value");

		m_CenterFrequency = Value;

		Update();
	}

	float GetCenterFrequency(void) const
	{
		return m_CenterFrequency;
	}

	//[MIN_FREQUENCY, MAX_FREQUENCY]
	void SetBandwidth(float Value)
	{
		ASSERT(MIN_FREQUENCY <= Value && Value <= MAX_FREQUENCY, "Invalid Value");

		m_Bandwidth = Value;

		Update();
	}
	float GetBandwidth(void) const
	{
		return m_Bandwidth;
	}

	double Process(double Value) override
	{
		return BiquadFilter::Process(Value);
	}

private:
	void Update(void)
	{
		BiquadFilter::SetBandStopFilterCoefficients(this, m_SampleRate, m_CenterFrequency, m_Bandwidth);
	}

private:
	uint32 m_SampleRate;
	float m_CenterFrequency;
	float m_Bandwidth;
};

#endif