#pragma once
#ifndef OSCILATOR_FILTER_H
#define OSCILATOR_FILTER_H

#include "Filter.h"
#include "../Math.h"
#include "../Debug.h"
#include "../Tables.h"

class OscillatorFilter : public Filter
{
#define MAX_OSCILLATOR_PHASE (TABLE_SIZE - 1)

public:
	OscillatorFilter(uint32 SampleRate)
		: m_SampleRate(SampleRate),
		  m_WaveTable(nullptr),
		  m_Freuency(0),
		  m_PhaseOffset(0),
		  m_PhaseStep(0),
		  m_Phase(0)
	{
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");

		SetWaveTable(SINE_TABLE);
		SetFrequency(20);
	}

	// Lenght must be equal to MAX_OSCILLATOR_PHASE
	void SetWaveTable(const float *Value)
	{
		ASSERT(Value != nullptr, "Invalid Value");

		m_WaveTable = Value;
	}
	const float *GetWaveTable(void) const
	{
		return m_WaveTable;
	}

	//[MIN_FREQUENCY, MAX_CUTOFF_FREQUENCY]
	void SetFrequency(float Value)
	{
		ASSERT(MIN_FREQUENCY <= Value && Value <= MAX_FREQUENCY, "Invalid Value");

		m_Freuency = Value;

		m_PhaseStep = m_Freuency * MAX_OSCILLATOR_PHASE / m_SampleRate;
	}
	float GetFrequency(void) const
	{
		return m_Freuency;
	}

	//[0, MAX_OSCILLATOR_PHASE]
	void SetPhaseOffset(float Value)
	{
		ASSERT(0 <= Value && Value <= MAX_OSCILLATOR_PHASE, "Invalid Value");

		m_PhaseOffset = Value;
	}
	float GetPhaseOffset(void) const
	{
		return m_PhaseOffset;
	}

	double Process(double Value) override
	{
		m_Phase = m_Phase + m_PhaseStep;
		if (m_Phase > MAX_OSCILLATOR_PHASE)
			m_Phase -= MAX_OSCILLATOR_PHASE;

		float currentPhase = m_Phase + m_PhaseOffset;
		if (currentPhase > MAX_OSCILLATOR_PHASE)
			currentPhase -= MAX_OSCILLATOR_PHASE;

		return Math::TableLookupLinear(m_WaveTable, currentPhase);
	}

private:
	uint32 m_SampleRate;
	const float *m_WaveTable;
	double m_Freuency;
	double m_PhaseOffset;

	double m_PhaseStep;
	float m_Phase;
};

#endif