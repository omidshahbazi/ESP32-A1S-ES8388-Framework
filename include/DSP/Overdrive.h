#pragma once
#ifndef OVERDRIVE_H
#define OVERDRIVE_H

#include "IDSP.h"
#include "../Math.h"
#include "../Debug.h"

class Overdrive : public IDSP
{
public:
	Overdrive(uint32 SampleRate)
		: m_Gain(0),
		  m_Drive(0)
	{
		SetGain(1);
		SetDrive(1);
	}

	//[0, 1]
	void SetGain(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Gain = Value;

		m_Multiplier = Math::Lerp(10.0, 80, m_Gain);
	}
	float GetGain(void)
	{
		return m_Gain;
	}

	//[0, 1]
	void SetDrive(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Drive = Value;

		m_Threshold = Math::Lerp(0.45, 0.23, m_Drive);
	}
	float GetDrive(void)
	{
		return m_Drive;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = Math::SymmetricalSoftClip(Buffer[i] * m_Multiplier, m_Threshold) / m_Multiplier;
	}

private:
	float m_Gain;
	float m_Drive;

	float m_Multiplier;
	float m_Threshold;
};

#endif