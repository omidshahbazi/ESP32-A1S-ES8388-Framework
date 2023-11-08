#pragma once
#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#include "Wave.h"

class LowPassFilter : public Wave
{
public:
	LowPassFilter(void)
		: m_SampleRate(44100),
		  m_Time(0),
		  m_CapacitorVoltage(0)
	{
		SetCutOffTime(1000);
	}

	void SetSampleRate(uint16 Value)
	{
		m_SampleRate = Value;

		Update();
	}
	uint16 GetSampleRate(void) const
	{
		return m_SampleRate;
	}

	void SetConstantTime(float Value)
	{
		m_Time = Value;

		Update();
	}
	void SetCutOffTime(float Value)
	{
		m_Time = 1 / (6.283 * Value);

		Update();
	}
	float GetTime(void) const
	{
		return m_Time;
	}

	float Process(float Value) override
	{
		float delta = (Value - m_CapacitorVoltage) / m_Devision;

		m_CapacitorVoltage += delta;

		return m_CapacitorVoltage;
	}

private:
	void Update(void)
	{
		m_Devision = m_Time * m_SampleRate;
	}

private:
	uint16 m_SampleRate;
	float m_Time;
	float m_Devision;
	float m_CapacitorVoltage;
};

#endif