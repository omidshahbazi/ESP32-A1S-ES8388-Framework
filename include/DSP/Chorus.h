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
		  m_Delay(SampleRate, MAX_DELAY_TIME),
		  m_SampleRate(SampleRate),
		  m_Depth(0),
		  m_Rate(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetDepth(1);
		SetRate(1);
		SetDelayTime(MAX_DELAY_TIME);
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

	//[0, 1]
	void SetRate(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Rate = Value;

		m_Oscillator.SetFrequency(Math::Lerp(0.01, 4, m_Rate));
	}
	float GetRate(void)
	{
		return m_Rate;
	}

	//[0, MAX_DELAY_TIME]
	void SetDelayTime(float Value)
	{
		m_Delay.SetTime(Value);
	}
	float GetDelayTime(void)
	{
		return m_Delay.GetTime();
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			m_Delay.Process(Buffer[i], false);

			float modulationIndex = m_Oscillator.Process() * m_Depth * m_SampleRate;

			float delayedSample = m_Delay.GetLerpedSample(modulationIndex, abs(modulationIndex - (int32)modulationIndex));

			Buffer[i] += delayedSample;
		}
	}

private:
	uint32 m_SampleRate;
	OscillatorFilter m_Oscillator;
	DelayFilter m_Delay;
	float m_Depth;
	float m_Rate;

public:
	static constexpr float MAX_DELAY_TIME = 0.025;
};

#endif
