#pragma once
#ifndef AUTO_WAH_H
#define AUTO_WAH_H

#include "Wah.h"
#include "../Math.h"
#include "../Filters/EnvelopeFollowerFilter.h"

class AutoWah : private Wah
{
public:
	AutoWah(uint32 SampleRate)
		: Wah(SampleRate),
		  m_EnvelopeFollowerFilter(SampleRate, 0.005, 0.02)
	{
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			float envelope = m_EnvelopeFollowerFilter.Process(Buffer[i]);

			Wah::SetRatio(envelope);

			Buffer[i] = Wah::Process(Buffer[i]);
		}
	}

private:
	EnvelopeFollowerFilter m_EnvelopeFollowerFilter;
};
#endif
