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
	float GetThreshold(void)
	{
		return m_NoiseGateFilter.GetThreshold();
	}

	//[0s, 1s]
	void SetAttackTime(float Value)
	{
		m_NoiseGateFilter.SetAttackTime(Value);
	}
	float GetAttackTime(void)
	{
		return m_NoiseGateFilter.GetAttackTime();
	}

	//[0s, 2s]
	void SetReleaseTime(float Value)
	{
		m_NoiseGateFilter.SetReleaseTime(Value);
	}
	float GetReleaseTime(void)
	{
		return m_NoiseGateFilter.GetReleaseTime();
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