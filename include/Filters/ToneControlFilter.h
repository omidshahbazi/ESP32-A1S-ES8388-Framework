#pragma once
#ifndef TONE_CONTROL_H
#define TONE_CONTROL_H

#include "../Math.h"
#include "LowPassFilter.h"
#include "HighPassFilter.h"

class ToneControl : public Wave
{
public:
	ToneControl(uint32 SampleRate)
		: m_LowPassFilter(SampleRate),
		  m_HighPassFilter(SampleRate),
		  m_Tone(1)
	{
		m_LowPassFilter.SetCutoffFrequency(408.0895981378369);	// Emulate big muff R39k and C10nF
		m_HighPassFilter.SetCutoffFrequency(1476.390939459707); // Emulate big muff R22k and C4.9nF
	}

	//[0, 1]
	void SetTone(float Value)
	{
		Value = Math::Clamp01(Value);

		m_Tone = Value;
	}
	float GetTone(void) const
	{
		return m_Tone;
	}

	double Process(double Value) override
	{
		return Math::Lerp(m_LowPassFilter.Process(Value), m_HighPassFilter.Process(Value), m_Tone);
	}

private:
	float m_Tone;

	LowPassFilter m_LowPassFilter;
	HighPassFilter m_HighPassFilter;
};
#endif