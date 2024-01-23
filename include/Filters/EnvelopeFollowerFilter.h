#pragma once
#ifndef ENVELOPE_FOLLOWER_FILTER_H
#define ENVELOPE_FOLLOWER_FILTER_H

#define METHOD_2

#include "Filter.h"
#include "../Math.h"
#include "../Debug.h"

class EnvelopeFollowerFilter : public Filter
{
public:
	EnvelopeFollowerFilter(uint32 SampleRate)
		: m_SampleRate(SampleRate),
		  m_AttackTime(0),
		  m_ReleaseTime(0),
		  m_AbsoluteValue(false),
		  m_Attack(0),
		  m_Release(0),
		  m_Envelope(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetAttackTime(0.00001);
		SetReleaseTime(0.00001);
		SetAbsoluteValue(false);
	}

	//(0, 0.2]
	void SetAttackTime(float Value)
	{
		ASSERT(0 < Value && Value <= 0.2, "Invalid Value");

		m_AttackTime = Value;

#if defined(METHOD_1)
		m_Attack = exp(-1 / (m_AttackTime * m_SampleRate));
#elif defined(METHOD_2)
		m_Attack = pow(0.01, 1 / (m_AttackTime * m_SampleRate));
#elif defined(METHOD_3)
		m_Attack = exp(log(0.5) / (m_AttackTime * m_SampleRate));
#endif
	}
	float GetAttackTime(void) const
	{
		return m_AttackTime;
	}

	//(0, 0.2]
	void SetReleaseTime(float Value)
	{
		ASSERT(0 < Value && Value <= 0.2, "Invalid Value");

		m_ReleaseTime = Value;

#if defined(METHOD_1)
		m_Release = exp(-1 / (m_ReleaseTime * m_SampleRate));
#elif defined(METHOD_2)
		m_Release = pow(0.01, 1 / (m_ReleaseTime * m_SampleRate));
#elif defined(METHOD_3)
		m_Release = exp(log(0.5) / (m_ReleaseTime * m_SampleRate));
#endif
	}
	float GetReleaseTime(void) const
	{
		return m_ReleaseTime;
	}

	void SetAbsoluteValue(bool Value)
	{
		m_AbsoluteValue = Value;
	}
	float GetAbsoluteValue(void) const
	{
		return m_AbsoluteValue;
	}

	double Process(double Value) override
	{
		Value = m_AbsoluteValue ? fabs(Value) : Value;

		double multiplier = 0;
		if (Value > m_Envelope)
			multiplier = m_Attack;
		else
			multiplier = m_Release;

#if defined(METHOD_1)
		m_Envelope += multiplier * (Value - m_Envelope);
#elif defined(METHOD_2)
		m_Envelope = multiplier * (m_Envelope - Value) + Value;
#elif defined(METHOD_3)
		m_Envelope = Math::Lerp(Value, m_Envelope, multiplier);
#endif

		return m_Envelope;
	}

protected:
	uint32 m_SampleRate;
	float m_AttackTime;
	float m_ReleaseTime;
	bool m_AbsoluteValue;

	double m_Attack;
	double m_Release;
	double m_Envelope;
};

#endif