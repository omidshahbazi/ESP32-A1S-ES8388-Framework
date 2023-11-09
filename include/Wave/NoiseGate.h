#pragma once
#ifndef WAVE_SHAPER_H
#define WAVE_SHAPER_H

#include "BiquadFilter.h"
#include "../Math.h"

class NoiseGate : public Wave
{
public:
	NoiseGate(void)
		: m_BiquadFilter(2),
		  m_LowerThreshold(0),
		  m_UpperThreshold(0),
		  m_Envelope(0)
	{
		static const float coef[] =
			{
				0.000002152381733479521, 0.000004304763466959042, 0.000002152381733479521, 1.9947405124091158, -0.9947486108316238, // b0, b1, b2, a1, a2
				0.0000019073486328125, 0.000003814697265625, 0.0000019073486328125, 1.997813341671618, -0.9978214525694677			// b0, b1, b2, a1, a2
			};

		m_BiquadFilter.SetCoef(coef);

		SetThreshold(0);
	}

	// 0 = -70dB, 1 = -10dB
	void SetThreshold(float val)
	{
		float dB = -70.0f + 60.0f * val;

		m_UpperThreshold = powf(10, dB / 20.0f);
		m_LowerThreshold = m_UpperThreshold / 2.0f;
	}

	double Process(double Value) override
	{
		m_Envelope = 1.4142 * m_BiquadFilter.Process(fabsf(Value));

		// detecting the gate and expansion area
		if (m_Envelope < m_LowerThreshold)
		{
			Value = 0;
		}
		else if (m_Envelope < m_UpperThreshold)
		{
			Value *= (m_Envelope - m_LowerThreshold) / (m_UpperThreshold - m_LowerThreshold);
		}

		return Value;
	}

private:
	BiquadFilter m_BiquadFilter;
	float m_LowerThreshold;
	float m_UpperThreshold;
	float m_Envelope;
};
#endif