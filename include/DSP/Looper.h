#pragma once
#ifndef LOOPER_H
#define LOOPER_H

#include "IDSP.h"
#include "../Filters/DelayFilter.h"

class Looper : public IDSP
{
public:
	enum class Modes
	{
		Replay = 0,
		Record,
		Additive
	};

public:
	Looper(uint32 SampleRate)
		: m_Delay(SampleRate, MAX_DELAY_TIME),
		  m_Mode(Modes::Replay),
		  m_Volume(0)
	{
		SetVolume(1);

		// SetTime(m_MaxTime);
	}

	void SetMode(Modes Value)
	{
		m_Mode = Value;
	}
	Modes GetMode(void) const
	{
		return m_Mode;
	}

	//[0, 1]
	void SetVolume(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Volume = Value;
	}
	float GetVolume(void) const
	{
		return m_Volume;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double input = Buffer[i];
			float delayLine = m_Delay.GetSample();

			if (m_Mode == Modes::Replay)
				m_Delay.MoveForward();
			else
				Buffer[i] = m_Delay.Process(input, (m_Mode == Modes::Additive));

			Buffer[i] = (input + (delayLine * m_Volume)) * 0.5;
		}
	}

private:
	DelayFilter m_Delay;
	Modes m_Mode;
	float m_Volume;

public:
	static constexpr float MAX_DELAY_TIME = 1; // 2.75;
};

#endif
