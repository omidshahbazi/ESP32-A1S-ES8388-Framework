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
		: m_SampleRate(0),
		  m_DelayTime(0),
		  m_Feedback(0),
		  m_DelayBuffer(nullptr),
		  m_DelayBufferLength(0),
		  m_DelayBufferIndex(0)
	{
		m_SampleRate = Math::Clamp(SampleRate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);

		m_DelayBuffer = Memory::Allocate<int16>(MAX_DELAY_TIME * m_SampleRate);

		SetDelayTime(0.5);
		SetFeedback(0.5);
	}

	~Reverb(void)
	{
		Memory::Deallocate(m_DelayBuffer);
	}

	//[0, 1]
	void SetDelayTime(float Value)
	{
		Value = Math::Clamp(Value, 0, MAX_DELAY_TIME);

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
		Value = Math::Clamp(Value, 0, 1);

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
			double delayedSample = m_DelayBuffer[m_DelayBufferIndex] / MAX_BUFFER_ELEMENT_COEFF;

			Buffer[i] += m_Feedback * delayedSample;

			m_DelayBuffer[m_DelayBufferIndex] = Buffer[i] * MAX_BUFFER_ELEMENT_COEFF;

			m_DelayBufferIndex = ++m_DelayBufferIndex % m_DelayBufferLength;
		}
	}

private:
	uint32 m_SampleRate;
	double m_DelayTime;
	double m_Feedback;

	int16 *m_DelayBuffer;
	uint32 m_DelayBufferLength;
	uint16 m_DelayBufferIndex = 0;

private:
	static constexpr float MAX_BUFFER_ELEMENT_COEFF = 1000.0;
	static constexpr float MAX_DELAY_TIME = 1;
};

#endif
