#pragma once
#ifndef SUSTAIN_H
#define SUSTAIN_H

#include "IDSP.h"
#include "../Filters/DelayFilter.h"

class Sustain : public IDSP
{
public:
	Sustain(uint32 SampleRate)
		: m_Delay(SampleRate, MAX_DELAY_TIME)
	{
		SetFeedback(1);
	}

	//[0, 1]
	void SetFeedback(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Delay.SetFeedback(Value);
	}
	float GetFeedback(void)
	{
		return m_Delay.GetFeedback();
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			float delayedSample = m_Delay.GetSample(m_Delay.GetBufferLength() / 2);

			Buffer[i] = m_Delay.Process(Buffer[i], false);
		}
	}

private:
	DelayFilter m_Delay;

	static constexpr float MAX_DELAY_TIME = 1;
};

#endif