// #pragma once
// #ifndef WAH_H
// #define WAH_H

// #include "IDSP.h"
// #include "../Filters/LowPassFilter.h"
// #include "../Tables.h"

// class Wah : public IDSP
// {
// public:
// 	Wah(uint32 SampleRate)
// 		: m_LowPassFilter(SampleRate),
// 		  m_Frequency(0),
// 		  m_SampleRate(0),
// 		  m_Step(0),
// 		  m_Position(0)
// 	{
// 		m_SampleRate = Math::Clamp(SampleRate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);

// 		SetCutoffFrequency(10000);
// 		SetFrequency(5000);
// 	}

// 	//[MIN_FREQUENCY, MAX_CUTOFF_FREQUENCY]
// 	void SetCutoffFrequency(float Value)
// 	{
// 		m_LowPassFilter.SetCutoffFrequency(Value);
// 	}
// 	float GetCutoffFrequency(void) const
// 	{
// 		return m_LowPassFilter.GetCutoffFrequency();
// 	}

// 	//[MIN_FREQUENCY_RESPONSE, MAX_FREQUENCY_RESPONSE]
// 	void SetFrequency(float Value)
// 	{
// 		Value = Math::Clamp(Value, MIN_FREQUENCY_RESPONSE, MAX_FREQUENCY_RESPONSE);

// 		m_Frequency = Value;

// 		m_Step = (m_Frequency * TABLE_SIZE) / m_SampleRate;
// 	}
// 	float GetFrequency(void) const
// 	{
// 		return m_Frequency;
// 	}

// 	void ProcessBuffer(double *Buffer, uint16 Count) override
// 	{
// 		for (uint16 i = 0; i < Count; ++i)
// 		{
// 			float modulator = SINE_TABLE[m_Position];

// 			Buffer[i] = m_LowPassFilter.Process(Buffer[i] * modulator);

// 			m_Position += m_Step;
// 			if (m_Position >= TABLE_SIZE)
// 				m_Position -= TABLE_SIZE;
// 		}
// 	}

// private:
// 	LowPassFilter m_LowPassFilter;
// 	float m_Frequency;
// 	uint32 m_SampleRate;
// 	int32 m_Step;
// 	int32 m_Position;

// public:
// 	static constexpr double MIN_FREQUENCY_RESPONSE = 200;
// 	static constexpr double MAX_FREQUENCY_RESPONSE = 20000;
// 	static constexpr double MAX_CUTOFF_FREQUENCY = LowPassFilter::MAX_CUTOFF_FREQUENCY;
// };

// #endif

#pragma once
#ifndef WAH_H
#define WAH_H

#include "IDSP.h"
#include "../Filters/BandPassFilter.h"
#include "../Tables.h"

// Cry-Baby 175Hz - 2500Hz 7.9
// Boutique 100Hz - 5000Hz 8-10
// Full Range 20Hz - 20kHz 7.9
class Wah : public IDSP
{
public:
	Wah(uint32 SampleRate)
		: m_BandPassFilter(),
		  m_Frequency(0),
		  m_SampleRate(0),
		  m_Step(0),
		  m_Phase(0)
	{
		m_SampleRate = Math::Clamp(SampleRate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);

		// SetCutoffFrequency(10000);
		SetFrequency(5000);
	}

	// //[MIN_FREQUENCY, MAX_CUTOFF_FREQUENCY]
	// void SetCutoffFrequency(float Value)
	// {
	// 	m_LowPassFilter.SetCutoffFrequency(Value);
	// }
	// float GetCutoffFrequency(void) const
	// {
	// 	return m_LowPassFilter.GetCutoffFrequency();
	// }

	//[MIN_FREQUENCY_RESPONSE, MAX_FREQUENCY_RESPONSE]
	void SetFrequency(float Value)
	{
		m_BandPassFilter.SetCenterFrequency(Value);

		Value = Math::Clamp(Value, MIN_FREQUENCY_RESPONSE, MAX_FREQUENCY_RESPONSE);

		m_Frequency = Value;

		m_Step = (m_Frequency * TABLE_SIZE) / m_SampleRate;
	}
	float GetFrequency(void) const
	{
		return m_Frequency;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			// // Simple WahWah effect: Modulate the frequency of a sinewave
			// double modulation = sin(m_Phase);
			// m_Phase += m_Step;

			// // Apply the modulation to the input signal
			// Buffer[i] *= modulation;

			// if (m_Phase >= TABLE_SIZE)
			// 	m_Phase -= TABLE_SIZE;

			Buffer[i] = m_BandPassFilter.Process(Buffer[i]);
		}

		// for (uint16 i = 0; i < Count; ++i)
		// {
		// 	// Apply the wah effect to each sample in the buffer
		// 	double inputSample = Buffer[i];

		// 	// Modulate the cutoff frequency using a sine wave
		// 	double modulation = sin(Math::TWO_PI_VALUE * m_Frequency * m_Position / m_SampleRate);
		// 	m_LowPassFilter.SetCutoffFrequency(m_LowPassFilter.GetCutoffFrequency() + modulation);

		// 	// Apply the wah effect using the low-pass filter
		// 	double outputSample = m_LowPassFilter.Process(inputSample);

		// 	// Store the result back to the buffer
		// 	Buffer[i] = outputSample;

		// 	// Update the position for the next sample
		// 	m_Position += m_Step;
		// 	if (m_Position >= TABLE_SIZE)
		// 	{
		// 		m_Position -= TABLE_SIZE;
		// 	}
		// }
	}

private:
	BandPassFilter m_BandPassFilter;
	float m_Frequency;
	uint32 m_SampleRate;
	float m_Step;
	float m_Phase;

public:
	static constexpr double MIN_FREQUENCY_RESPONSE = 200;
	static constexpr double MAX_FREQUENCY_RESPONSE = 20000;
};

#endif