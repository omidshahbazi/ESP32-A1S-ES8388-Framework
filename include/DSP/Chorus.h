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
		  m_CurrentPhase(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		m_DelayBuffer = Memory::Allocate<int16>(MAX_DELAY_TIME * m_SampleRate, true);

		SetDepth(0.5);
		SetRate(0.5);
		SetDelayTime(0.5);
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
			double modulation = m_Depth * sin(Math::TWO_PI_VALUE * m_Rate * m_CurrentPhase);
			m_CurrentPhase += 1.0 / m_SampleRate;
			if (m_CurrentPhase > 1.0)
				m_CurrentPhase -= 1.0;

			double delayedSample = GetDelayedSample(m_DelayTime + modulation);

			Buffer[i] = (Buffer[i] + delayedSample) / 2.0;

			m_DelayBuffer[m_DelayBufferIndex] = Buffer[i] * MAX_BUFFER_ELEMENT_COEFF;

			m_DelayBufferIndex = ++m_DelayBufferIndex % m_DelayBufferLength;
		}
	}

private:
	double GetDelayedSample(double delayTime)
	{
		uint32 index = delayTime * m_SampleRate;
		double fraction = (delayTime * m_SampleRate) - index;

		uint32 indexDelayed = (m_DelayBufferIndex + index) % m_DelayBufferLength;
		uint32 indexNext = (indexDelayed + 1) % m_DelayBufferLength;

		return Math::Lerp((double)m_DelayBuffer[indexDelayed], m_DelayBuffer[indexNext], fraction) / MAX_BUFFER_ELEMENT_COEFF;
	}

private:
	uint32 m_SampleRate;
	double m_Depth;
	double m_Rate;
	double m_DelayTime;

	int16 *m_DelayBuffer;
	uint32 m_DelayBufferLength;
	uint16 m_DelayBufferIndex;

	double m_CurrentPhase;

	static constexpr double MAX_BUFFER_ELEMENT_COEFF = 32767.0;
	static constexpr float MAX_DELAY_TIME = 1;
};

#endif