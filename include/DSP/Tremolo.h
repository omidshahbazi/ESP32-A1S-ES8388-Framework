#pragma once
#ifndef TREMOLO_H
#define TREMOLO_H

#include "IDSP.h"
#include "../Math.h"
#include "../Debug.h"

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
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetDepth(0.5F);
		SetRate(5);
	}

	//[0, 1]
	void SetDepth(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Depth = Value;
	}
	float GetDepth(void)
	{
		return m_Depth;
	}

	//[MIN_FREQUENCY, MAX_FREQUENCY]
	void SetRate(float Value)
	{
		ASSERT(MIN_FREQUENCY <= Value && Value <= MAX_FREQUENCY, "Invalid Value");

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
