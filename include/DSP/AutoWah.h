#pragma once
#ifndef AUTO_WAH_H
#define AUTO_WAH_H

#include "IDSP.h"
#include "../Math.h"
#include "../Filters/EnvelopeFollowerFilter.h"
#include "../Filters/BandPassFilter.h"

// TODO: Theoretically, this can inherit from Wah DSP and set something on it to achieve the same effect automatically, So first fix the Wah, then tackle this one
class AutoWah : public IDSP
{
public:
	AutoWah(uint32 SampleRate)
		: m_EnvelopeFollowerFilter(SampleRate, 0.005, 0.02),
		  m_BandPassFilter(SampleRate)
	{
		m_BandPassFilter.SetFrequencies(175, 2500);
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double envelope = m_EnvelopeFollowerFilter.Process(Buffer[i]);

			// Adjust the filter frequency based on the envelope
			double filterFrequency = Math::Lerp(.0, 2500, envelope);
			m_BandPassFilter.SetCenterFrequency(filterFrequency);

			// Process the input sample through the bandpass filter
			double outputSample = m_BandPassFilter.Process(Buffer[i]);

			// Replace the input sample with the processed output
			Buffer[i] = outputSample;
		}
	}

private:
	EnvelopeFollowerFilter m_EnvelopeFollowerFilter;
	BandPassFilter m_BandPassFilter;
};
#endif
