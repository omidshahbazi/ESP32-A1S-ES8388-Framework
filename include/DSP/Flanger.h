#pragma once
#ifndef FLANGER_H
#define FLANGER_H

#include "IDSP.h"
#include "../Math.h"
#include "../Filters/OscillatorFilter.h"
#include "../Filters/DelayFilter.h"

class Flanger : public IDSP
{
public:
	Flanger(uint32 SampleRate)
		: m_Oscillator(SampleRate),
		  m_Delay(SampleRate, MAX_DELAY_TIME * 2),
		  m_SampleRate(SampleRate),
		  m_Depth(0),
		  m_WetRate(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetDepth(1);
		SetRate(1);
		SetFeedback(0.5);
		SetWetRate(0.5);
	}

	//[0, MAX_DEPTH]
	void SetDepth(float Value)
	{
		ASSERT(0 <= Value && Value <= MAX_DEPTH, "Invalid Value");

		m_Depth = Value;
	}
	float GetDepth(void) const
	{
		return m_Depth;
	}

	//(0.01, 4]
	void SetRate(float Value)
	{
		ASSERT(0 < Value && Value <= 4, "Invalid Value");

		m_Oscillator.SetFrequency(Value);
	}
	float GetRate(void) const
	{
		return m_Oscillator.GetFrequency();
	}

	//[0, 1]
	void SetFeedback(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Delay.SetFeedback(Value);
	}
	float GetFeedback(void) const
	{
		return m_Delay.GetFeedback();
	}

	//[0, 1]
	void SetWetRate(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_WetRate = Value;
	}
	float GetWetRate(void)
	{
		return m_WetRate;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			m_Delay.Process(Buffer[i], true);

			float modulationIndex = abs(m_Oscillator.Process()) * m_Depth;

			float delayedSample = m_Delay.GetLerpedSample(modulationIndex, modulationIndex - (int32)modulationIndex);

			Buffer[i] = Math::Lerp(Buffer[i], delayedSample, m_WetRate);
		}
	}

private:
	uint32 m_SampleRate;
	OscillatorFilter m_Oscillator;
	DelayFilter m_Delay;
	float m_Depth;
	float m_WetRate;

public:
	static constexpr float MAX_DELAY_TIME = 0.025;
	static constexpr float MAX_DEPTH = 100;
};

#endif
