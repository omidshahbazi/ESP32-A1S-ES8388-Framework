#pragma once
#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "IDSP.h"
#include "../Debug.h"

class Compressor : public IDSP
{
public:
	Compressor(void)
	{
		SetThreshold(-0.9);
		SetRatio(2);
	}

	//[-1, 1]
	void SetThreshold(float Value)
	{
		ASSERT(-1 <= Value && Value <= 1, "Invalid Value");

		m_Threshold = Value;
	}
	float GetThreshold(void)
	{
		return m_Threshold;
	}

	//(0, 2]
	void SetRatio(float Value)
	{
		ASSERT(0 < Value && Value <= 2, "Invalid Value");

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
			if (Buffer[i] > m_Threshold)
				Buffer[i] = m_Threshold + (Buffer[i] - (m_Threshold / m_Ratio));
		}
	}

private:
	float m_Threshold;
	float m_Ratio;
};

#endif
