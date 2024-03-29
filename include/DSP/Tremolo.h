#pragma once
#ifndef TREMOLO_H
#define TREMOLO_H

#include "IDSP.h"
#include "../Math.h"
#include "../Debug.h"
#include "../Filters/OscillatorFilter.h"

class Tremolo : public IDSP
{

public:
	Tremolo(uint32 SampleRate)
		: m_Oscillator(SampleRate),
		  m_Depth(0)
	{
		SetDepth(0.5);
		SetRate(1);
	}

	//[0, 1]
	void SetDepth(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Depth = Value;
	}
	float GetDepth(void) const
	{
		return m_Depth;
	}

	//(0.01, 25]
	void SetRate(float Value)
	{
		ASSERT(0 < Value && Value <= 25, "Invalid Value");

		m_Oscillator.SetFrequency(Value);
	}
	float GetRate(void) const
	{
		return m_Oscillator.GetFrequency();
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			float modulation = (1 - m_Depth) + (m_Depth * 0.5 * (1 + m_Oscillator.Process()));

			Buffer[i] *= modulation;
		}
	}

private:
	OscillatorFilter m_Oscillator;
	float m_Depth;
};

#endif
