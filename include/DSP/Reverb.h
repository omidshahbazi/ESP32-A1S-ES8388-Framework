#pragma once
#ifndef REVERB_H
#define REVERB_H

#include "IDSP.h"
#include "../Math.h"
#include "../Memory.h"

class Reverb : public IDSP
{
public:
	Reverb(uint32 SampleRate)
		: m_SampleRate(SampleRate),
		  m_DelayTime(0),
		  m_Feedback(0),
		  m_DelayBuffer(nullptr),
		  m_DelayBufferLength(0),
		  m_DelayBufferIndex(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		m_DelayBuffer = Memory::Allocate<float>(MAX_DELAY_TIME * m_SampleRate, true);

		SetDelayTime(0.5);
		SetFeedback(0.5);
	}

	~Reverb(void)
	{
		Memory::Deallocate(m_DelayBuffer);
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

	//[0, 1]
	void SetFeedback(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Feedback = Value;
	}
	float GetFeedback(void)
	{
		return m_Feedback;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double delayedSample = m_DelayBuffer[m_DelayBufferIndex];

			Buffer[i] += m_Feedback * delayedSample;

			m_DelayBuffer[m_DelayBufferIndex] = Buffer[i];

			m_DelayBufferIndex = ++m_DelayBufferIndex % m_DelayBufferLength;
		}
	}

private:
	uint32 m_SampleRate;
	float m_DelayTime;
	float m_Feedback;

	float *m_DelayBuffer;
	uint32 m_DelayBufferLength;
	uint32 m_DelayBufferIndex;

public:
	static constexpr float MAX_DELAY_TIME = 1;
};

#endif
