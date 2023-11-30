#pragma once
#ifndef NOISE_GATE_FILTER_H
#define NOISE_GATE_FILTER_H

#include "Filter.h"
#include "../Math.h"

class NoiseGateFilter : public Filter
{
public:
	NoiseGateFilter(uint32 SampleRate)
		: m_SampleRate(0),
		  m_Threshold(0),
		  m_AttackTime(0),
		  m_ReleaseTime(0),
		  m_AttackCoefficient(0),
		  m_ReleaseCoefficient(0),
		  m_Envelope(0)
	{
		m_SampleRate = Math::Clamp(SampleRate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);

		SetThreshold(0.5F);
		SetAttackTime(0.01);
		SetReleaseTime(0.1);
	}

	//[0, 1]
	void SetThreshold(float Value)
	{
		Value = Math::Clamp01(Value);

		m_Threshold = Value;
	}
	float GetThreshold(void)
	{
		return m_Threshold;
	}

	//[0s, 10s]
	void SetAttackTime(float Value)
	{
		Value = Math::Clamp(Value, 0, 10);

		m_AttackTime = Value;

		m_AttackCoefficient = exp(-1 / (m_SampleRate * m_AttackTime));
	}
	float GetAttackTime(void)
	{
		return m_AttackTime;
	}

	//[0s, 10s]
	void SetReleaseTime(float Value)
	{
		Value = Math::Clamp(Value, 0, 10);

		m_ReleaseTime = Value;

		m_ReleaseCoefficient = exp(-1 / (m_SampleRate * m_ReleaseTime));
	}
	float GetReleaseTime(void)
	{
		return m_ReleaseTime;
	}

	double Process(double Value) override
	{
		double valueAbs = fabs(Value);

		m_Envelope = (valueAbs > m_Envelope) ? (m_AttackCoefficient * m_Envelope + (1 - m_AttackCoefficient) * valueAbs) : (m_ReleaseCoefficient * m_Envelope);

		if (m_Envelope < m_Threshold)
			return Value * (m_Envelope / m_Threshold);

		return Value;
	}

private:
	uint32 m_SampleRate;
	float m_Threshold;
	double m_AttackTime;
	double m_ReleaseTime;
	double m_AttackCoefficient;
	double m_ReleaseCoefficient;
	double m_Envelope;
};
#endif