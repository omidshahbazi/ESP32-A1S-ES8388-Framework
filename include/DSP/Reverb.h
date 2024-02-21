#pragma once
#ifndef REVERB_H
#define REVERB_H

#include "IDSP.h"
#include "../Filters/DelayFilter.h"

class Reverb : public IDSP
{
public:
	Reverb(uint32 SampleRate)
		: m_Delay(SampleRate, MAX_DELAY_TIME)
	{
		SetDelayTime(0.5);
		SetFeedback(0.5);
	}

	//[0, MAX_DELAY_TIME]
	void SetDelayTime(float Value)
	{
		m_Delay.SetTime(Value);
	}
	float GetDelayTime(void) const
	{
		return m_Delay.GetTime();
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

		m_Delay.SetOutputMixRate(Value);
	}
	float GetWetRate(void) const
	{
		return m_Delay.GetOutputMixRate();
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = m_Delay.Process(Buffer[i], true);
	}

private:
	DelayFilter m_Delay;

public:
	static constexpr float MAX_DELAY_TIME = 1;
};

#endif
