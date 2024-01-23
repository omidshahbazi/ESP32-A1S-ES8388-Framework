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
		float Min;
		float Max;
		float Resonance;
		float Low;
		float High;
	};

	const FrequencyRange FREQUENCY_RANGES[3] = {
		{175, 2.5 * KHz, 7.9, 175, 2 * KHz}, // Cry Baby 175Hz - 2500Hz 7.9
		{100, 5 * KHz, 9, 400, 4 * KHz},	 // Boutique 100Hz - 5000Hz 8-10
		{20, 20 * KHz, 7.9, 500, 19 * KHz}}; // Full Range 20Hz - 20kHz 7.9

public:
	enum Types
	{
		CryBaby = 0,
		// Boutique = 1,
		// FullRange = 2
	};

public:
	Wah(uint32 SampleRate)
		: m_BandPassFilter(SampleRate)
	{
		SetType(Types::CryBaby);
	}

	void SetType(Types Value)
	{
		m_Type = Value;

		const FrequencyRange &freqRange = FREQUENCY_RANGES[m_Type];

		m_BandPassFilter.SetFrequencies(freqRange.Min, freqRange.Max);
		m_BandPassFilter.SetResonance(freqRange.Resonance);
	}
	Types GetType(void) const
	{
		return m_Type;
	}

	//[0, 1]
	void SetRatio(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Ratio = Value;

		const FrequencyRange &freqRange = FREQUENCY_RANGES[m_Type];

		m_BandPassFilter.SetCenterFrequency(Math::Lerp(freqRange.Low, freqRange.High, m_Ratio));
	}
	float GetRatio(void) const
	{
		return m_Ratio;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = Process(Buffer[i]);
	}

protected:
	double Process(double Value)
	{
		return m_BandPassFilter.Process(Value);
	}

private:
	BandPassFilter m_BandPassFilter;
	Types m_Type;
	float m_Ratio;
};

#endif