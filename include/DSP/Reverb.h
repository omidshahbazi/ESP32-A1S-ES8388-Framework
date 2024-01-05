#pragma once
#ifndef REVERB_H
#define REVERB_H

#include "IDSP.h"
#include "../Filters/DelayFilter.h"

class Reverb : public IDSP, private DelayFilter
{
public:
	Reverb(uint32 SampleRate)
		: DelayFilter(SampleRate, MAX_DELAY_TIME)
	{
		SetDelayTime(0.5);
		SetFeedback(0.5);
	}

	//[0, MAX_DELAY_TIME]
	void SetDelayTime(float Value)
	{
		ASSERT(0 <= Value && Value <= MAX_DELAY_TIME, "Invalid Value");

		DelayFilter::SetTime(Value);
	}
	float GetDelayTime(void)
	{
		return DelayFilter::GetTime();
	}

	//[0, 1]
	void SetFeedback(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		DelayFilter::SetFeedback(Value);
	}
	float GetFeedback(void)
	{
		return DelayFilter::GetFeedback();
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = DelayFilter::Process(Buffer[i], true);
	}

public:
	static constexpr float MAX_DELAY_TIME = 1;
};

#endif
