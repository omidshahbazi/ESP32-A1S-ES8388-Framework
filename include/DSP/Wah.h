#pragma once
#ifndef WAH_H
#define WAH_H

#include "IDSP.h"
#include "../Filters/EnvelopeFollowerFilter.h"
#include "../Filters/BandPassFilter.h"

// Cry-Baby 175Hz - 2500Hz 7.9
// Boutique 100Hz - 5000Hz 8-10
// Full Range 20Hz - 20kHz 7.9
class Wah : public IDSP
{
public:
	Wah(uint32 SampleRate)
		: m_EnvelopeFollowerFilter(SampleRate, 0.005, 0.02),
		  m_BandPassFilter(SampleRate)
	{
		m_BandPassFilter.SetFrequencies(175, 2500);
	}

	void SetFrequency(float Value)
	{
		m_Frequency = Value;

		m_BandPassFilter.SetBandwidth(1 + (Value * 2325));

		// m_Step = (m_Frequency * TABLE_SIZE) / m_SampleRate;
	}
	float GetFrequency(void) const
	{
		return m_Frequency;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			// Apply the Wah effect to each sample in the buffer
			double inputSample = Buffer[i];

			// double modulation = sin(Math::TWO_PI_VALUE * m_Frequency * m_Position / m_SampleRate);

			double outputSample = m_BandPassFilter.Process(inputSample);

			Buffer[i] = outputSample;

			// m_Position += m_Step;
			// if (m_Position >= TABLE_SIZE)
			// {
			// 	m_Position -= TABLE_SIZE;
			// }
		}
	}

private:
	EnvelopeFollowerFilter m_EnvelopeFollowerFilter;
	BandPassFilter m_BandPassFilter;
	float m_Frequency;
	uint32 m_SampleRate;
	float m_Step;
};

#endif

// void ProcessBuffer(double *Buffer, uint16 Count) override
// {
// 	for (uint16 i = 0; i < Count; ++i)
// 	{
// 		// // Simple WahWah effect: Modulate the frequency of a sinewave
// 		// double modulation = sin(m_Phase);
// 		// m_Phase += m_Step;

// 		// // Apply the modulation to the input signal
// 		// Buffer[i] *= modulation;

// 		// if (m_Phase >= TABLE_SIZE)
// 		// 	m_Phase -= TABLE_SIZE;

// 		Buffer[i] = m_LowPassFilter.Process(Buffer[i]);
// 	}

// 	// for (uint16 i = 0; i < Count; ++i)
// 	// {
// 	// 	// Apply the wah effect to each sample in the buffer
// 	// 	double inputSample = Buffer[i];

// 	// 	// Modulate the cutoff frequency using a sine wave
// 	// 	double modulation = sin(Math::TWO_PI_VALUE * m_Frequency * m_Position / m_SampleRate);
// 	// 	m_LowPassFilter.SetCutoffFrequency(m_LowPassFilter.GetCutoffFrequency() + modulation);

// 	// 	// Apply the wah effect using the low-pass filter
// 	// 	double outputSample = m_LowPassFilter.Process(inputSample);

// 	// 	// Store the result back to the buffer
// 	// 	Buffer[i] = outputSample;

// 	// 	// Update the position for the next sample
// 	// 	m_Position += m_Step;
// 	// 	if (m_Position >= TABLE_SIZE)
// 	// 	{
// 	// 		m_Position -= TABLE_SIZE;
// 	// 	}
// 	// }
// }

// private:
// BandPassFilter m_LowPassFilter;
// float m_Frequency;
// uint32 m_SampleRate;
// float m_Step;
// float m_Phase;
