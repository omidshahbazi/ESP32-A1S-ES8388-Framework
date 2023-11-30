#pragma once
#ifndef OSCILLATOR_FILTER_H
#define OSCILLATOR_FILTER_H

#include "Filter.h"
#include "../Tables.h"
#include "Notes.h"

class OscillatorFilter : public Filter
{
public:
	Oscillator(uint32 SampleRate)
		: m_SampleRate(0),
		  m_Frequency(NOTE_G5),
		  m_PhaseShift(0),
		  m_WaveTable(nullptr),
		  m_Phase(0),
		  m_PhaseIncrement(0)
	{
		m_SampleRate = Math::Clamp(SampleRate, MIN_SAMPLE_RATE, MAX_SAMPLE_RATE);

		Update();
	}

	//[MIN_FREQUENCY, MAX_FREQUENCY]
	void SetFrequency(float Value)
	{
		Value = Math::Clamp(Value, MIN_FREQUENCY, MAX_FREQUENCY);

		m_Frequency = Value;

		Update();
	}
	float GetFrequency(void) const
	{
		return m_Frequency;
	}

	void SetPhaseShift(float Value)
	{
		m_PhaseShift = Value;
	}
	float GetPhaseShift(void) const
	{
		return m_PhaseShift;
	}

	// A table from Tables.h
	void SetWaveTable(const float *Table)
	{
		m_WaveTable = Table;
	}
	const float *GetWaveTable(void) const
	{
		return m_WaveTable;
	}

	float GetPhase(void) const
	{
		return m_Phase;
	}

	double Process(double Value) override
	{
		if (m_WaveTable == nullptr)
			return 0;

		m_Phase += m_PhaseIncrement + m_PhaseShift;
		if (m_Phase >= TABLE_SIZE)
			m_Phase -= TABLE_SIZE;

		return Math::TableLookupLinear(m_WaveTable, m_Phase) + Value;
	}

private:
	void Update(void)
	{
		m_PhaseIncrement = m_Frequency * (TABLE_SIZE - 1) / (float)m_SampleRate;
	}

private:
	uint16 m_SampleRate;
	float m_Frequency;
	float m_PhaseShift;
	const float *m_WaveTable;
	float m_Phase;
	float m_PhaseIncrement;
};

#endif