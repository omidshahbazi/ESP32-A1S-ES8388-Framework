#pragma once
#ifndef AUTO_WAH_H
#define AUTO_WAH_H

#include "IDSP.h"
#include "../Math.h"
#include "../Filters/BandPassFilter.h"

class AutoWah : public IDSP
{
public:
	AutoWah(void)
	{
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double inputSample = Buffer[i];

			// Apply envelope follower (you may need to implement this)
			double envelope = CalculateEnvelope(inputSample);

			// Adjust the filter frequency based on the envelope
			double filterFrequency = Math::Lerp(m_MinFrequency, m_MaxFrequency, envelope);
			m_BandPassFilter.SetCenterFrequency(filterFrequency);

			// Process the input sample through the bandpass filter
			double outputSample = m_BandPassFilter.Process(inputSample);

			// printf("envelope: %f, filterFrequency: %f, outputSample: %f\n", envelope, filterFrequency, outputSample);

			// Replace the input sample with the processed output
			Buffer[i] = outputSample;
		}
	}

	double CalculateEnvelope(double inputSample)
	{
		// Simple envelope follower: absolute value of the input sample
		return std::abs(inputSample);
	}

private:
	BandPassFilter m_BandPassFilter;
	double m_MinFrequency = 200.0; // Adjust these values as needed
	double m_MaxFrequency = 2000.0;
};
#endif
