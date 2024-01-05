#pragma once
#ifndef REVERB_H
#define REVERB_H

#include "IDSP.h"
#include "../Filters/DelayFilter.h"

class Reverb : public IDSP
{
public:
	Reverb(uint32 SampleRate)
		: m_DelayFilter(SampleRate, MAX_DELAY_TIME)
	{
		SetDelayTime(0.5);
		SetFeedback(0.5);
	}

	//[0, MAX_DELAY_TIME]
	void SetDelayTime(float Value)
	{
		ASSERT(0 <= Value && Value <= MAX_DELAY_TIME, "Invalid Value");

		m_DelayFilter.SetTime(Value);
	}
	float GetDelayTime(void)
	{
		return m_DelayFilter.GetTime();
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
			Buffer[i] = m_DelayFilter.Process(Buffer[i], true);
	}

private:
	DelayFilter m_DelayFilter;

public:
	static constexpr float MAX_DELAY_TIME = 1;
};

#endif
