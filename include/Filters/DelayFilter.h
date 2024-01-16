#pragma once
#ifndef DELAY_FILTER_H
#define DELAY_FILTER_H

#include "Filter.h"
#include "../Math.h"
#include "../Memory.h"

class DelayFilter : public Filter
{
public:
	DelayFilter(uint32 SampleRate, float MaxTime)
		: m_SampleRate(SampleRate),
		  m_MaxTime(MaxTime),
		  m_Time(0),
		  m_Feedback(0),
		  m_Buffer(nullptr),
		  m_BufferLength(0),
		  m_BufferIndex(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");
		ASSERT(MaxTime * SampleRate * sizeof(float) <= SAMPLE_RATE_22050 * sizeof(float), "The result of MaxTime * SampleRate * sizeof(float) must not exceed from %i", SAMPLE_RATE_22050 * sizeof(float));

		m_Buffer = Memory::Allocate<float>(m_MaxTime * m_SampleRate, true);

		SetTime(m_MaxTime);
		SetFeedback(1);
	}

	~DelayFilter(void)
	{
		Memory::Deallocate(m_Buffer);
	}

	//[0, MaxTime]
	void SetTime(float Value)
	{
		ASSERT(0 <= Value && Value <= m_MaxTime, "Invalid Value");

		m_Time = Value;

		m_BufferLength = Math::Max(m_Time * m_SampleRate, 1);
	}
	float GetTime(void)
	{
		return m_Time;
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

	uint32 GetBufferLength(void) const
	{
		return m_BufferLength;
	}

	float GetSample(int32 Offset) const
	{
		return GetCircularSample(m_BufferIndex + Offset);
	}

	float GetLerpedSample(int32 Offset, float Fraction) const
	{
		int32 index = m_BufferIndex + Offset;

		return Math::Lerp(GetCircularSample(index), GetCircularSample(index + 1), Fraction);
	}

	double Process(double Value, bool Additive)
	{
		float delayedSample = GetCircularSample(m_BufferIndex);

		m_Buffer[m_BufferIndex] = Value;

		if (Additive)
			m_Buffer[m_BufferIndex] += delayedSample;

		m_BufferIndex = (m_BufferIndex + 1) % m_BufferLength;

		return Value + delayedSample;
	}

private:
	float GetCircularSample(int32 Index) const
	{
		Index = Index % m_BufferLength;
		if (Index < 0)
			Index = m_BufferLength - Index;

		return m_Buffer[Index] * m_Feedback;
	}

	double Process(double Value) override
	{
		return 0;
	}

private:
	uint32 m_SampleRate;
	float m_MaxTime;
	float m_Time;
	float m_Feedback;

	float *m_Buffer;
	uint32 m_BufferLength;
	uint32 m_BufferIndex;
};

#endif