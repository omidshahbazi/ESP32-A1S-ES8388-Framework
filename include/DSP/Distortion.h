#pragma once
#ifndef DISTORTION_H
#define DISTORTION_H

#include "IDSP.h"
#include "../Math.h"
#include "../Debug.h"
#include "../Filters/LowPassFilter.h"
#include "../Filters/NoiseGateFilter.h"

class Distortion : public IDSP
{
public:
	Distortion(uint32 SampleRate)
		: m_Gain(0),
		  m_Rate(0),
		  m_Factor(0),
		  m_Multiplier(0)
	{
		SetGain(1);
		SetRate(1);
	}

	//[0, 1]
	void SetGain(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Gain = Value;

		m_Factor = Math::Lerp(0.001, 0.9, m_Gain);

		printf("Gain %f\n", m_Factor);

		// 0.001000
	}
	float GetGain(void)
	{
		return m_Gain;
	}

	//[0, 1]
	void SetRate(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Rate = Value;

		m_Multiplier = Math::Lerp(100.0, 1500, m_Rate);

		printf("Rate %f\n", m_Multiplier);

		// 693.623718
	}
	float GetRate(void)
	{
		return m_Rate;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = Math::ExponentialSaturation(Buffer[i] * m_Multiplier, m_Factor) / m_Multiplier;
	}

private:
	float m_Gain;
	float m_Rate;

	float m_Factor;
	float m_Multiplier;
};

#endif