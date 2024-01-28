#pragma once
#ifndef AUTO_WAH_H
#define AUTO_WAH_H

#include "Wah.h"
#include "../Math.h"
#include "../Filters/EnvelopeFollowerFilter.h"

class AutoWah : private Wah
{
#define ENVELOP_AVERAGE_DURATION 0.1

public:
	AutoWah(uint32 SampleRate)
		: Wah(SampleRate),
		  m_EnvelopeFollowerFilter(SampleRate),
		  m_MaxEnvelopSampleCount(ENVELOP_AVERAGE_DURATION * SampleRate),
		  m_EnvelopeSum(0),
		  m_EnvelopeSampleCount(0)
	{
		m_EnvelopeFollowerFilter.SetAttackTime(0.015841);
		m_EnvelopeFollowerFilter.SetReleaseTime(0.045401);
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			m_EnvelopeSum += abs(m_EnvelopeFollowerFilter.Process(Buffer[i]));
			if (++m_EnvelopeSampleCount == m_MaxEnvelopSampleCount)
			{
				float ratio = m_EnvelopeSum / m_EnvelopeSampleCount;
				ratio *= 125;
				ratio = Math::Clamp01(ratio);

				Wah::SetRatio(ratio);

				m_EnvelopeSum = 0;
				m_EnvelopeSampleCount = 0;
			}

			Buffer[i] = Wah::Process(Buffer[i]);
		}
	}

private:
	EnvelopeFollowerFilter m_EnvelopeFollowerFilter;
	uint32 m_MaxEnvelopSampleCount;
	double m_EnvelopeSum;
	uint32 m_EnvelopeSampleCount;
};
#endif
