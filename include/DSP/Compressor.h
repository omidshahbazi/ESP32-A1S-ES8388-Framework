#pragma once
#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "IDSP.h"

class Compressor : public IDSP
{
public:
	Compressor(voide)
	{
		SetThreshold(-0.9);
		SetRatio(2);
	}

	//[0, 1]
	void SetThreshold(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Threshold = Value;
	}
	float GetThreshold(void)
	{
		return m_Threshold;
	}

	//[0, 1]
	void SetRatio(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Ratio = Value;
	}
	float GetRatio(void)
	{
		return m_Ratio;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			if (input > m_Threshold)
				Buffer[i] = (m_Threshold - 1) + (Buffer[i] - (m_Threshold - 1) / (m_Ratio - 1));
		}
	}

private:
	float m_Threshold;
	float m_Ratio;
};

#endif
