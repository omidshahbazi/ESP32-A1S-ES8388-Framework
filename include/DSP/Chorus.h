#pragma once
#ifndef CHORUS_H
#define CHORUS_H

#include "IDSP.h"
#include "../Math.h"
#include "../Filters/OscillatorFilter.h"
#include "../Filters/DelayFilter.h"

class Chorus : public IDSP
{
public:
	Chorus(uint32 SampleRate)
		: m_Oscillator(SampleRate),
		  m_Delay(SampleRate, MAX_DELAY_TIME * 2),
		  m_SampleRate(SampleRate),
		  m_Depth(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetDepth(1);
		SetRate(1);
	}

	//[0, MAX_DEPTH]
	void SetDepth(float Value)
	{
		ASSERT(0 <= Value && Value <= MAX_DEPTH, "Invalid Value");

		m_Depth = Value;
	}
	float GetDepth(void)
	{
		return m_Depth;
	}

	//(0, 4]
	void SetRate(float Value)
	{
		ASSERT(0 < Value && Value <= 4, "Invalid Value");

		m_Oscillator.SetFrequency(Value);
	}
	float GetRate(void)
	{
		return m_Oscillator.GetFrequency();
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			m_Delay.Process(Buffer[i], false);

			float modulationIndex = abs(m_Oscillator.Process()) * m_Depth;

			float delayedSample = m_Delay.GetLerpedSample(modulationIndex, modulationIndex - (int32)modulationIndex);

			Buffer[i] = (Buffer[i] + delayedSample) * 0.5;
		}
	}

private:
	uint32 m_SampleRate;
	OscillatorFilter m_Oscillator;
	DelayFilter m_Delay;
	float m_Depth;

public:
	static constexpr float MAX_DELAY_TIME = 0.025;
	static constexpr float MAX_DEPTH = 100;
};

#endif
