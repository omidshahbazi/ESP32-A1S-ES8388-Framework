#pragma once
#ifndef WAH_H
#define WAH_H

#include "IDSP.h"
#include "../Filters/BandPassFilter.h"

class Wah : public IDSP
{
private:
	struct FrequencyRange
	{
	public:
		float Low;
		float High;
		float Resonance;
		float RatioMultiplier;
	};

public:
	enum Types
	{
		CryBaby = 0,
		Boutique = 1,
		FullRange = 2
	};

public:
	Wah(uint32 SampleRate) : m_BandPassFilter(SampleRate)
	{
		SetType(Types::CryBaby);
	}

	void SetType(Types Value)
	{
		m_Type = Value;

		const FrequencyRange &freqRange = FREQUENCY_RANGES[m_Type];

		m_BandPassFilter.SetFrequencies(freqRange.Low, freqRange.High);
		m_BandPassFilter.SetResonance(freqRange.Resonance);
	}
	Types GetType(void) const
	{
		return m_Type;
	}

	void SetRatio(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Ratio = Value;

		const FrequencyRange &freqRange = FREQUENCY_RANGES[m_Type];

		m_BandPassFilter.SetCenterFrequency(Math::Lerp(freqRange.Low, freqRange.High, Value * freqRange.RatioMultiplier));
	}
	float GetRatio(void) const
	{
		return m_Ratio;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = m_BandPassFilter.Process(Buffer[i]);
	}

private:
	BandPassFilter m_BandPassFilter;
	Types m_Type;
	float m_Ratio;

	const FrequencyRange FREQUENCY_RANGES[3] = {
		{175, 2.5 * KHz, 7.9, 0.5}, // Cry Baby 175Hz - 2500Hz 7.9
		{100, 5 * KHz, 9, 0.5},		// Boutique 100Hz - 5000Hz 8-10
		{20, 20 * KHz, 7.9, 0.5}};	// Full Range 20Hz - 20kHz 7.9
};

#endif