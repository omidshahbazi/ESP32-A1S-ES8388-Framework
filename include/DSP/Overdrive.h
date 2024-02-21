#pragma once
#ifndef OVERDRIVE_H
#define OVERDRIVE_H

#include "IDSP.h"
#include "../Math.h"
#include "../Debug.h"
#include "../Filters/LowPassFilter.h"

class Overdrive : public IDSP
{
public:
	Overdrive(void)
		: m_Gain(0),
		  m_Drive(0),
		  m_Factor(0),
		  m_Multiplier(0)
	{
		SetGain(1);
		SetDrive(1);
	}

	//[0, 1]
	void SetGain(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Gain = Value;

		m_Factor = Math::Lerp(2.0, 4.0, m_Gain);
	}
	float GetGain(void) const
	{
		return m_Gain;
	}

	//[0, 1]
	void SetDrive(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Drive = Value;

		m_Multiplier = Math::Lerp(100.0, 200, m_Drive);
	}
	float GetDrive(void) const
	{
		return m_Drive;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = Math::SoftClip(Buffer[i] * m_Multiplier, m_Factor) / m_Multiplier;
	}

private:
	float m_Gain;
	float m_Drive;

	float m_Factor;
	float m_Multiplier;
};

#endif