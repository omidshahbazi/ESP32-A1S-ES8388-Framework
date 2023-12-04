#pragma once
#ifndef WAH_H
#define WAH_H

#include "IDSP.h"
#include "../Filters/BandPassFilter.h"

class Wah : public IDSP
{
public:
	Wah(uint32 SampleRate)
	{
		// Set initial values for the Wah effect
		m_BandPassFilter.SetCenterFrequency(1162.5); // Initial center frequency, you can adjust this
		m_BandPassFilter.SetBandwidth(1000);		 // Initial bandwidth, you can adjust this
		m_EnvelopePos = 0.0f;
		m_EnvelopeDirection = 1;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			// Apply the Wah effect to each sample in the buffer
			double inputSample = Buffer[i];

			// Modulate the center frequency using an envelope
			float envelopeRate = 0.001f; // Adjust the rate of the envelope
			m_EnvelopePos += m_EnvelopeDirection * envelopeRate;
			if (m_EnvelopePos >= 1.0f || m_EnvelopePos <= 0.0f)
			{
				m_EnvelopeDirection *= -1; // Change direction when reaching envelope limits
			}

			float centerFrequency = 1162.5 + 500.0f * m_EnvelopePos; // Adjust the range and modulation as needed
			m_BandPassFilter.SetCenterFrequency(centerFrequency);

			// Process the input sample through the BandPassFilter
			double outputSample = m_BandPassFilter.Process(inputSample);
			Buffer[i] = outputSample;
		}
	}

private:
	BandPassFilter m_BandPassFilter;
	float m_EnvelopePos;
	int m_EnvelopeDirection;
};

#endif
