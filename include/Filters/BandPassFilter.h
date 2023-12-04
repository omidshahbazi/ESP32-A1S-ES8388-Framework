#pragma once
#ifndef BAND_PASS_FILTER_H
#define BAND_PASS_FILTER_H

#include "BiquadFilter.h"
#include "../Math.h"

class BandPassFilter : private BiquadFilter
{
public:
	BandPassFilter(void)
		: BiquadFilter(1),
		  m_CenterFrequency(1),
		  m_Bandwidth(1)
	{
		SetBandwidth(2000);
		SetCenterFrequency(0.5 * MAX_FREQUENCY);
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
		BiquadFilter::SetBandPassFilterCoefficients(this, m_CenterFrequency, m_Bandwidth);
	}

private:
	float m_CenterFrequency;
	float m_Bandwidth;
};

#endif