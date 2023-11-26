#pragma once
#ifndef WAH__H
#define WAH__H

#include "IDSP.h"
#include "../Wave/LowPassFilter.h"
#include "../Tables.h"
#include <stdio.h>

class Wah : public IDSP
{
public:
	Wah(uint32 SampleRate)
		: m_LowPassFilter(SampleRate),
		  m_Frequency(0),
		  m_SampleRate(0),
		  m_Step(0),
		  m_Position(0)
	{
		m_SampleRate = Math::Clamp(SampleRate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);

		SetCutoffFrequency(10000);
		SetFrequency(5000);
	}

	void SetCutoffFrequency(float Value)
	{
		m_LowPassFilter.SetCutoffFrequency(Value);
	}

	float GetCutoffFrequency(void) const
	{
		return m_LowPassFilter.GetCutoffFrequency();
	}

	void SetFrequency(float Value)
	{
		Value = Math::Clamp(Value, 200, 20000);

		m_Frequency = Value;

		m_Step = (m_Frequency * TABLE_SIZE) / m_SampleRate;
	}

	float GetFrequency(void) const
	{
		return m_Frequency;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			float modulator = SINE_TABLE[m_Position];

			Buffer[i] = m_LowPassFilter.Process(Buffer[i] * modulator);

			m_Position += m_Step;
			if (m_Position >= TABLE_SIZE)
				m_Position -= TABLE_SIZE;
		}
	}

private:
	LowPassFilter m_LowPassFilter;
	float m_Frequency;
	uint32 m_SampleRate;
	int32 m_Step;
	int32 m_Position;
};

#endif