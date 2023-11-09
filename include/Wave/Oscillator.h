#pragma once
#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "Wave.h"
#include "Tables.h"

class Oscillator : public Wave
{
public:
	Oscillator(void)
		: m_SampleRate(44100),
		  m_Frequency(700),
		  m_PhaseShift(0),
		  m_WaveTable(nullptr),
		  m_Phase(0),
		  m_PhaseIncrement(0)
	{
		Update();
	}

	void SetSampleRate(uint16 Value)
	{
		m_SampleRate = Value;

		Update();
	}
	uint16 GetSampleRate(void) const
	{
		return m_SampleRate;
	}

	void SetFrequency(float Value)
	{
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

		return LookupLinear(m_WaveTable, m_Phase);
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