#pragma once
#ifndef DELAY_H
#define DELAY_H

#include "IDSP.h"
#include "../Memory.h"

class Delay : public IDSP
{
public:
	Delay(uint8 FrameLength, uint32 SampleRate)
		: m_Buffer(nullptr),
		  m_BufferLength(FrameLength * 2),
		  m_BufferIndex(FrameLength),
		  m_DelayTime(0),
		  m_SampleRate(SampleRate)
	{
		m_Buffer = Memory::Allocate<double>(m_BufferLength);
	}

	~DelayEffect(void)
	{
		Memory::Deallocate(m_Buffer);
	}

	void SetDelayTime(float Value)
	{
		m_DelayTime = Value;
		m_DelaySamples = static_cast<uint16>(m_SampleRate * m_DelayTime);
	}

	float GetDelayTime(void) const
	{
		return m_DelayTime;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double input = Buffer[i];
			uint8 delayIndex = (m_BufferIndex + m_DelaySamples) % m_BufferLength;

			Buffer[i] = m_Buffer[delayIndex];

			m_Buffer[delayIndex] = input;

			m_BufferIndex = (m_BufferIndex + 1) % m_BufferLength;
		}
	}

private:
	double *m_Buffer;
	uint8 m_BufferLength;
	uint8 m_BufferIndex;
	float m_DelayTime;
	uint32 m_SampleRate;
	uint16 m_DelaySamples;
};

#endif