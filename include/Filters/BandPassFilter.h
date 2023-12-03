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
		  m_CenterFrequency(0),
		  m_Bandwidth(0)
	{
		SetBandwidth(0.1);
		SetCenterFrequency(0.5 * MAX_FREQUENCY);
	}

	//[MIN_FREQUENCY, MAX_FREQUENCY]
	void SetCenterFrequency(float Value)
	{
		Value = Math::Clamp(Value, MIN_FREQUENCY, MAX_FREQUENCY);

		m_CenterFrequency = Value;

		Update();
	}

	float GetCenterFrequency(void) const
	{
		return m_CenterFrequency;
	}

	//(0, 1]
	void SetBandwidth(float Value)
	{
		Value = Math::ClampExcluded0To1(Value);

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
		float w0 = Math::TWO_PI_VALUE * m_CenterFrequency;

		float cosw0 = cosf(w0);
		float sinw0 = sinf(w0);

		float alpha = sinw0 / (2 * m_Bandwidth);

		Coefficients coeffs;
		coeffs.a1 = (1 + alpha) / 2;
		coeffs.a2 = 0;
		coeffs.b0 = -(1 + alpha) / 2;
		coeffs.b1 = -2 * cosw0;
		coeffs.b2 = 1;

		// coeffs.a1 /= coeffs.b0;
		// coeffs.a2 /= coeffs.b0;
		// coeffs.b0 /= coeffs.b0;
		// coeffs.b1 /= coeffs.b0;
		// coeffs.b2 /= coeffs.b0;

		SetCoefficients(&coeffs);
	}

private:
	float m_CenterFrequency;
	float m_Bandwidth;
};

#endif