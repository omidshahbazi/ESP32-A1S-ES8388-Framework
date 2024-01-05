#pragma once
#ifndef TEST_H
#define TEST_H

#include "IDSP.h"
#include "../Filters/ThreeToneControlFilter.h"

class Test : public IDSP
{
public:
	Test(uint32 SampleRate)
		: m_ThreeToneControlFilter(SampleRate)
	{
	}

	void SetLowTone(float Value)
	{
		m_ThreeToneControlFilter.SetLowTone(Value);
	}

	void SetMidTone(float Value)
	{
		m_ThreeToneControlFilter.SetMidTone(Value);
	}

	void SetHighTone(float Value)
	{
		m_ThreeToneControlFilter.SetHighTone(Value);
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			Buffer[i] = m_ThreeToneControlFilter.Process(Buffer[i]);
		}
	}

private:
	ThreeToneControlFilter m_ThreeToneControlFilter;
};

#endif