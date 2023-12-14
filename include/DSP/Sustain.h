#pragma once
#ifndef SUSTAIN_H
#define SUSTAIN_H

#include "IDSP.h"
#include "../Memory.h"

class Sustain : public IDSP
{
public:
	Sustain(uint32 SampleRate)
		: m_SampleRate(SampleRate),
		  m_Feedback(0),
		  m_Buffer(nullptr),
		  m_BufferLength(0),
		  m_BufferIndex(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		m_BufferLength = MAX_DELAY_TIME * m_SampleRate;
		m_Buffer = Memory::Allocate<float>(m_BufferLength, true);

		SetFeedback(1);
	}

	~Sustain(void)
	{
		delete[] m_Buffer;
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
			double delayedSample = m_Buffer[(m_BufferIndex + m_BufferLength / 2) % m_BufferLength];

			double outputSample = Buffer[i] + m_Feedback * delayedSample;

			m_Buffer[m_BufferIndex] = Buffer[i];
			m_BufferIndex = (m_BufferIndex + 1) % m_BufferLength;

			Buffer[i] = outputSample;
		}
	}

private:
	uint32 m_SampleRate;
	float m_Feedback;

	float *m_Buffer;
	uint32 m_BufferLength;
	uint32 m_BufferIndex;

	static constexpr float MAX_DELAY_TIME = 1;
};

#endif