#pragma once
#ifndef TEST__H
#define TEST__H

#include "IDSP.h"
#include <math.h>

class Test : public IDSP
{
public:
	Test(void)
		: m_Value(0.001F)
	{
	}

	void SetValue(float Value)
	{
		m_Value = Value;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] -= fmod(Buffer[i], 0.1F * m_Value);
	}

private:
	float m_Value;
};

#endif