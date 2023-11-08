#pragma once
#ifndef TONE_CONTROL_H
#define TONE_CONTROL_H

#include "../Math.h"
#include "LowPassFilter.h"
#include "HighPassFilter.h"

class ToneControl : public Wave
{
public:
	ToneControl(void)
		: m_Tone(0.5F)
	{
		m_LowPassFilter.SetConstantTime(0.00039F);	  // Emulate big muff R39k and C10nF
		m_HighPassFilter.SetConstantTime(0.0001078F); // Emulate big muff R22k and C4.9nF
	}

	void SetTone(float Value)
	{
		m_Tone = Value;
	}
	float GetTone(void) const
	{
		return m_Tone;
	}

	float Process(float Value) override
	{
		// return (m_Tone * m_HighPassFilter.Process(Value)) + ((1 - m_Tone) * m_LowPassFilter.Process(Value));
		return Math::Lerp(m_LowPassFilter.Process(Value), m_HighPassFilter.Process(Value), m_Tone);
	}

private:
	float m_Tone = 1.0f;

	LowPassFilter m_LowPassFilter;
	HighPassFilter m_HighPassFilter;
};
#endif