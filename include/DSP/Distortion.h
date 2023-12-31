#pragma once
#ifndef DISTORTION_H
#define DISTORTION_H

#include "IDSP.h"
#include "../Math.h"
#include "../Debug.h"

class Distortion : public IDSP
{
public:
	Distortion(uint32 SampleRate)
		: m_Rate(0)
	{
		SetRate(1);
	}

	//[0, 1]
	void SetRate(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Overdrive", "Invalid Value");

		m_Rate = Value;
	}
	float GetRate(void)
	{
		return m_Rate;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = Math::ExponentialSaturation(Buffer[i], (m_Rate + 1) * 100);
	}

private:
	float m_Rate;
};

#endif