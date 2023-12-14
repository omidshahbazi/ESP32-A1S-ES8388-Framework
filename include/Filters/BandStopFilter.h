#pragma once
#ifndef BAND_STOP_FILTER_H
#define BAND_STOP_FILTER_H

#include "BiquadFilter.h"
#include "../Math.h"
#include "../Debug.h"

class BandStopFilter : private BiquadFilter
{
public:
	BandStopFilter(uint32 SampleRate)
		: BiquadFilter(1),
		  m_SampleRate(SampleRate),
		  m_CenterFrequency(1),
		  m_Bandwidth(1),
		  m_Resonance(1)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetFrequencies(1950, 2050);
		SetResonance(1);
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

	//[MIN_FREQUENCY, MAX_FREQUENCY]
	void SetFrequencies(float Min, float Max)
	{
		ASSERT(MIN_FREQUENCY <= Min && Min <= MAX_FREQUENCY, "Invalid Min");
		ASSERT(MIN_FREQUENCY <= Max && Max <= MAX_FREQUENCY, "Invalid Max");

		m_Bandwidth = Max - Min;
		m_CenterFrequency = m_Bandwidth / 2;

		Update();
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
		BiquadFilter::SetBandStopFilterCoefficients(this, m_SampleRate, m_CenterFrequency, m_Bandwidth, m_Resonance);
	}

private:
	uint32 m_SampleRate;
	float m_CenterFrequency;
	float m_Bandwidth;
	float m_Resonance;
};

#endif