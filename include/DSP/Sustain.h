#pragma once
#ifndef SUSTAIN_H
#define SUSTAIN_H

#include "IDSP.h"
#include "../Filters/DelayFilter.h"

class Sustain : public IDSP
{
public:
	Sustain(uint32 SampleRate)
		: m_DelayFilter(SampleRate, MAX_DELAY_TIME),
		  m_BufferLength(m_DelayFilter.GetBufferLength())
	{
		SetFeedback(1);
	}

	//[0, 1]
	void SetFeedback(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_DelayFilter.SetFeedback(Value);
	}
	float GetFeedback(void)
	{
		return m_DelayFilter.GetFeedback();
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			float delayedSample = m_DelayFilter.GetDelayedSample(m_BufferLength / 2);

			Buffer[i] = m_DelayFilter.Process(Buffer[i], false);
		}
	}

private:
	DelayFilter m_DelayFilter;
	uint32 m_BufferLength;

	static constexpr float MAX_DELAY_TIME = 1;
};

#endif