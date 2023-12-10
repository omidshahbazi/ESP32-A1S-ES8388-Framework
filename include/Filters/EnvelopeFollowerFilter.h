#pragma once
#ifndef ENVELOPE_FOLLOWER_FILTER_H
#define ENVELOPE_FOLLOWER_FILTER_H

#include "Filter.h"
#include "../Math.h"
#include "../Debug.h"

class EnvelopeFollowerFilter : public Filter
{
public:
	EnvelopeFollowerFilter(uint32 SampleRate, double AttackTime, double ReleaseTime)
		: m_Attack(0),
		  m_Release(0),
		  m_Envelope(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");
		ASSERT(0 <= AttackTime && AttackTime <= 1, "Invalid AttackTime");
		ASSERT(0 <= ReleaseTime && ReleaseTime <= 1, "Invalid ReleaseTime");

		m_Attack = pow(0.01, 1 / (AttackTime * SampleRate));
		m_Release = pow(0.01, 1 / (ReleaseTime * SampleRate));
	}

	double Process(double Value) override
	{
		double absValue = fabs(Value);

		if (absValue > m_Envelope)
			m_Envelope = m_Attack * (m_Envelope - absValue) + absValue;
		else
			m_Envelope = m_Release * (m_Envelope - absValue) + absValue;

		return m_Envelope;
	}

protected:
	double m_Attack;
	double m_Release;
	double m_Envelope;
};

#endif