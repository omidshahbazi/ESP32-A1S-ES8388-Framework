#pragma once
#ifndef DISTORTION_H
#define DISTORTION_H

#include "IDSP.h"
#include "../Math.h"
#include "../Debug.h"
#include "../Filters/WaveShaperFilter.h"

class Distortion : public IDSP
{
public:
	Distortion(uint32 SampleRate)
		: m_Gain(0),
		  m_Rate(0),
		  m_Factor(0),
		  m_Multiplier(0)
	{
		static WaveShaperFilter::TablePoints points[] = {{-1, -0.8}, {-0.5, -0.8}, {0, 0}, {0.5, 0.8}, {1, 0.8}};
		m_WaveShaperFilter.SetTable(points, 5);

		SetGain(1);
		SetRate(1);
	}

	//[0, 1]
	void SetGain(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Gain = Value;

		m_Factor = Math::Lerp(1.0, 4, m_Gain);
	}
	float GetGain(void) const
	{
		return m_Gain;
	}

	//[0, 1]
	void SetRate(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Rate = Value;

		m_Multiplier = Math::Lerp(50.0, 400, m_Rate);
	}
	float GetRate(void) const
	{
		return m_Rate;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = m_Factor * m_WaveShaperFilter.Process(Buffer[i] * m_Multiplier) / m_Multiplier;
	}

private:
	WaveShaperFilter m_WaveShaperFilter;

	float m_Gain;
	float m_Rate;

	float m_Factor;
	float m_Multiplier;
};

#endif