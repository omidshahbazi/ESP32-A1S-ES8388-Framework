#pragma once
#ifndef CHORUS_H
#define CHORUS_H

#include "IDSP.h"
#include "../Math.h"
#include "../Memory.h"

class Chorus : public IDSP
{
public:
	Chorus(uint32 SampleRate)
		: m_SampleRate(SampleRate),
		  m_Depth(0),
		  m_Rate(0),
		  m_DelayTime(0),
		  m_DelayBuffer(nullptr),
		  m_DelayBufferLength(0),
		  m_DelayBufferIndex(0),
		  m_DeltaPhase(0),
		  m_Phase(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		m_DelayBuffer = Memory::Allocate<float>(MAX_DELAY_TIME * m_SampleRate, true);

		SetDepth(0.5);
		SetRate(0.5);
		SetDelayTime(0);
	}

	~Chorus(void)
	{
		Memory::Deallocate(m_DelayBuffer);
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

		m_DeltaPhase = Math::TWO_PI_VALUE * Math::Lerp(1.0, 25, m_Rate) / m_SampleRate;
	}
	float GetRate(void)
	{
		return m_Rate;
	}

	//[0, MAX_DELAY_TIME]
	void SetDelayTime(float Value)
	{
		ASSERT(0 <= Value && Value <= MAX_DELAY_TIME, "Invalid Value");

		m_DelayTime = Value;

		m_DelayBufferLength = Math::Max(m_DelayTime * m_SampleRate, 1);
	}
	float GetDelayTime(void)
	{
		return m_DelayTime;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			float modulation = m_Depth * sin(m_Phase);

			float delayedSample = GetDelayedSample(m_DelayTime);

			Buffer[i] += delayedSample;

			m_DelayBuffer[m_DelayBufferIndex] = Buffer[i];

			m_DelayBufferIndex = ++m_DelayBufferIndex % m_DelayBufferLength;

			m_Phase += m_DeltaPhase;
			if (m_Phase >= Math::TWO_PI_VALUE)
				m_Phase -= Math::TWO_PI_VALUE;
		}
	}

private:
	float GetDelayedSample(float DelayTime)
	{
		// uint32 index = DelayTime * m_SampleRate;
		// double fraction = (DelayTime * m_SampleRate) - index;

		// uint32 indexDelayed = (m_DelayBufferIndex + index) % m_DelayBufferLength;
		// uint32 indexNext = (indexDelayed + 1) % m_DelayBufferLength;

		// return Math::Lerp(m_DelayBuffer[indexDelayed], m_DelayBuffer[indexNext], fraction);

		return m_DelayBuffer[(uint32)(DelayTime * m_SampleRate)];
	}

private:
	uint32 m_SampleRate;
	float m_Depth;
	float m_Rate;
	float m_DelayTime;

	float *m_DelayBuffer;
	uint32 m_DelayBufferLength;
	uint16 m_DelayBufferIndex;

	float m_DeltaPhase;
	float m_Phase;

public:
	static constexpr float MAX_DELAY_TIME = 1; // 0.050;
};

#endif