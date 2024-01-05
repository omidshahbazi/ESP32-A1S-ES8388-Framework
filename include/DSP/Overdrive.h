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
	Overdrive(uint32 SampleRate)
		: m_LowPassFilter(SampleRate),
		  m_Drive(0)
	{
		m_LowPassFilter.SetCutoffFrequency(2 * KHz);
		m_LowPassFilter.SetDeltaTime(0.1);
		SetDrive(1);
	}

	//[0, 1]
	void SetDrive(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Drive = Value;
	}
	float GetDrive(void)
	{
		return m_Drive;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		float drive = Math::Max(m_Drive, 0.01);

		for (uint16 i = 0; i < Count; ++i)
		{
			double input = m_LowPassFilter.Process(Buffer[i]);

			input = Math::SoftClip(input, drive * 50);

			Buffer[i] = Math::Clamp(input, -1, 1);
		}
	}

private:
	LowPassFilter m_LowPassFilter;
	float m_Drive;
};

#endif