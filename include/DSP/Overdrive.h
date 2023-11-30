#pragma once
#ifndef OVERDRIVE_H
#define OVERDRIVE_H

#include "IDSP.h"
#include "../Math.h"

class Overdrive : public IDSP
{
public:
	Overdrive(void)
		: m_Drive(1)
	{
	}

	//[0, 1]
	void SetDrive(float Value)
	{
		Value = Math::Clamp01(Value);

		m_Drive = Value;
	}
	float GetDrive(void)
	{
		return m_Drive;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = tanh(Buffer[i] * (2 + (m_Drive * 50)));
	}

private:
	float m_Drive;
};

#endif