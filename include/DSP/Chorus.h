#pragma once
#ifndef CHORUS_H
#define CHORUS_H

#include "IDSP.h"
#include "../Math.h"

#include <vector>

class Chorus : public IDSP
{
public:
	Chorus(uint32 SampleRate)
		: m_SampleRate(0),
		  m_Depth(0.5),
		  m_Rate(0.5),
		  m_Delay(0.5),
		  m_CurrentPhase(0.0)
	{
		m_SampleRate = Math::Clamp(SampleRate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);
		m_DelayBuffer.resize(static_cast<size_t>(m_SampleRate * m_Delay), 0.0);
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			// Calculate modulation phase
			double modulation = m_Depth * sin(Math::TWO_PI_VALUE * m_Rate * m_CurrentPhase);

			// Apply chorus effect (create delayed copies)
			double delayedSample = GetDelayedSample(m_Delay + modulation);
			Buffer[i] = (Buffer[i] + delayedSample) / 2.0;

			// Update modulation phase
			m_CurrentPhase += 1.0 / m_SampleRate;
			if (m_CurrentPhase > 1.0)
			{
				m_CurrentPhase -= 1.0;
			}

			// Update delay buffer
			m_DelayBuffer[m_DelayBufferIndex] = Buffer[i];
			m_DelayBufferIndex = (m_DelayBufferIndex + 1) % m_DelayBuffer.size();
		}
	}

private:
	uint32 m_SampleRate;
	double m_Depth; // Modulation depth
	double m_Rate;	// Modulation rate
	double m_Delay; // Base delay time in seconds
	double m_CurrentPhase;

	std::vector<float> m_DelayBuffer;
	size_t m_DelayBufferIndex = 0;

	// Function to get a delayed sample from the delay buffer
	double GetDelayedSample(double delayTime)
	{
		// Interpolation for fractional delay
		size_t index = static_cast<size_t>(delayTime * m_SampleRate);
		double fraction = delayTime * m_SampleRate - index;

		size_t indexDelayed = (m_DelayBufferIndex + index) % m_DelayBuffer.size();
		size_t indexNext = (indexDelayed + 1) % m_DelayBuffer.size();

		return Math::Lerp(m_DelayBuffer[indexDelayed], m_DelayBuffer[indexNext], fraction);
	}
};

#endif