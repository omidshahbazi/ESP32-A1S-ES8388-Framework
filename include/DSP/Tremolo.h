#pragma once
#ifndef TREMOLO_H
#define TREMOLO_H

#include "IDSP.h"
#include "../Math.h"

class Tremolo : public IDSP
{

public:
	Tremolo(uint32 SampleRate)
		: m_SampleRate(0),
		  m_Depth(0),
		  m_Rate(0),
		  m_DeltaPhase(0),
		  m_Phase(0)
	{
		m_SampleRate = Math::Clamp(SampleRate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);

		SetDepth(0.5F);
		SetRate(5);
	}

	//[0, 1]
	void SetDepth(float Value)
	{
		Value = Math::Clamp01(Value);

		m_Depth = Value;
	}
	float GetDepth(void)
	{
		return m_Depth;
	}

	//[MIN_FREQUENCY, MAX_FREQUENCY]
	void SetRate(float Value)
	{
		Value = Math::Clamp(Value, MIN_FREQUENCY, MAX_FREQUENCY);

		m_Rate = Value;

		Update();
	}
	float GetRate(void)
	{
		return m_Rate;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double modulation = (1 - m_Depth) + (m_Depth * 0.5 * (1 + sin(m_Phase)));

			Buffer[i] *= modulation;

			m_Phase += m_DeltaPhase;

			if (m_Phase >= Math::TWO_PI_VALUE)
				m_Phase -= Math::TWO_PI_VALUE;
		}
	}

private:
	void Update(void)
	{
		m_DeltaPhase = Math::TWO_PI_VALUE * m_Rate / m_SampleRate;
	}

private:
	float m_Depth;
	float m_Rate;
	uint32 m_SampleRate;

	double m_DeltaPhase;
	double m_Phase;
};

#endif
