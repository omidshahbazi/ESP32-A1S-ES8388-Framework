#pragma once
#ifndef NOISE_GATE_H
#define NOISE_GATE_H

#include "IDSP.h"
#include "../Filters/NoiseGateFilter.h"

class NoiseGate : public IDSP
{
public:
	NoiseGate(uint32 SampleRate)
		: m_NoiseGateFilter(SampleRate)
	{
	}

	//[0, 1]
	void SetThreshold(float Value)
	{
		m_NoiseGateFilter.SetThreshold(Value);
	}
	float GetThreshold(void) const
	{
		return m_NoiseGateFilter.GetThreshold();
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = m_NoiseGateFilter.Process(Buffer[i]);
	}

private:
	NoiseGateFilter m_NoiseGateFilter;
};

#endif