#pragma once
#ifndef AUTO_WAH_H
#define AUTO_WAH_H

#include "IDSP.h"
#include "../Math.h"

class AutoWah : public IDSP
{
public:
	AutoWah(uint32 SampleRate)
	{
		m_SampleRate = Math::Clamp(SampleRate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);
		m_Envelope = 0.0;
		m_CurrentWahFreq = 0.0; // You might want to set an initial value here
		CalculateFilterCoefficients();
		m_PrevInput = 0.0;
		m_PrevOutput = 0.0;
	}

	void SetFrequency_TOTUNE(float Value)
	{
		Value = Math::Clamp(Value, MIN_FREQUENCY, MAX_FREQUENCY);

		m_CurrentWahFreq = Value;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double inputSample = Buffer[i];

			// Envelope follower
			double target = std::fabs(inputSample);
			m_Envelope += (target - m_Envelope) * (inputSample > m_Envelope ? m_AttackCoeff : m_ReleaseCoeff);

			// Update filter cutoff frequency based on the envelope
			double wahFreq = m_WahMin + (m_WahMax - m_WahMin) * m_Envelope;

			// Update filter coefficients if the frequency has changed
			if (wahFreq != m_CurrentWahFreq)
			{
				m_CurrentWahFreq = wahFreq;
				CalculateFilterCoefficients();
			}

			// Resonant low-pass filter
			double w = 2.0 * M_PI * m_CurrentWahFreq / m_SampleRate;
			double g = tan(w * 0.5) / m_Q;
			double h = 1.0 / (1.0 + g);

			// Update filter state
			double filtered = h * (inputSample + m_PrevInput - m_Q * m_PrevOutput);
			m_PrevInput = inputSample;
			m_PrevOutput = filtered;

			// Store the processed sample back to the buffer
			Buffer[i] = filtered;
		}
	}

private:
	void CalculateFilterCoefficients()
	{
		double w = 2.0 * M_PI * m_CurrentWahFreq / m_SampleRate;
		double g = tan(w * 0.5) / m_Q;
		double h = 1.0 / (1.0 + g);

		// Update filter coefficients
		m_AttackCoeff = exp(-1.0 / (m_SampleRate * m_AttackTime));
		m_ReleaseCoeff = exp(-1.0 / (m_SampleRate * m_ReleaseTime));
	}

	// Add member variables for parameters
	double m_SampleRate;
	double m_AttackTime = 0.01; // You might want to set initial values
	double m_ReleaseTime = 0.1;
	double m_WahMin = 400.0;
	double m_WahMax = 1000.0;
	double m_Q = 1.0;

	double m_Envelope;
	double m_CurrentWahFreq;

	double m_AttackCoeff;
	double m_ReleaseCoeff;

	double m_PrevInput;
	double m_PrevOutput;
};

#endif
