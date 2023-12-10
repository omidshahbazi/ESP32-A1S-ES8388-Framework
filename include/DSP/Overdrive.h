#pragma once
#ifndef OVERDRIVE_H
#define OVERDRIVE_H

#include "IDSP.h"
#include "../Math.h"
#include "../Filters/HighPassFilter.h"
#include "../Debug.h"

class Overdrive : public IDSP
{
public:
	Overdrive(void)
		: m_Drive(0)
	{
		SetDrive(1);

		m_HighPassFilter.SetCutoffFrequency(20);
	}

	//[0, 1]
	void SetDrive(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Overdrive", "Invalid Value");

		m_Drive = Value;
	}
	float GetDrive(void)
	{
		return m_Drive;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double value = tanh(Buffer[i] * (2 + (m_Drive * 50)));

			value = m_HighPassFilter.Process(value);

			Buffer[i] = value;
		}
	}

private:
	float m_Drive;
	HighPassFilter m_HighPassFilter;
};

#endif