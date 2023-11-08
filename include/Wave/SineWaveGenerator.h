#pragma once
#ifndef SINE_WAVE_GENERATOR_H
#define SINE_WAVE_GENERATOR_H

#include "../include/ESP32A1SCodec.h"
#include <math.h>

template <typename T>
class SineWaveGenerator
{
public:
	SineWaveGenerator(uint16 SampleRate = 44100, float Frequency = 700, float Amplitude = 8000)
		: m_SampleRate(SampleRate), m_Frequency(Frequency), m_Amplitude(Amplitude), m_Buffer(nullptr), m_BufferLength(0)
	{
		SetupWave();
	}

	void SetSampleRate(uint16 Value)
	{
		Value = Math::Clamp(Value, 1, 192000);

		m_SampleRate = Value;

		SetupWave();
	}

	uint16 GetSampleRate(void) const
	{
		return m_SampleRate;
	}

	void SetFrequency(float Value)
	{
		Value = Math::Clamp(Value, 1, 20000);

		m_Frequency = Value;

		SetupWave();
	}

	float GetFrequency(void) const
	{
		return m_Frequency;
	}

	void SetAmplitude(float Value)
	{
		Value = Math::Clamp(Value, 15, 15000);

		m_Amplitude = Value;

		SetupWave();
	}

	float GetAmplitude(void) const
	{
		return m_Amplitude;
	}

	const T *GetBuffer(void) const
	{
		return m_Buffer;
	}

	uint16 GetBufferLength(void) const
	{
		return m_BufferLength;
	}

private:
	void SetupWave(void)
	{
		const uint16 SamplePerCycle = m_SampleRate / m_Frequency;

		if (m_Buffer != nullptr)
			Deallocate(m_Buffer);

		m_BufferLength = SamplePerCycle * 2;
		m_Buffer = Allocate<T>(m_BufferLength);

		const int32 Mask = 0x0FFFFFFF >> (sizeof(int32) - sizeof(T));
		for (int32 i = 0; i < m_BufferLength; i += 2)
		{
			double sinVal = sin(i / 2 * 2 * Math::PI / SamplePerCycle) * m_Amplitude;

			m_Buffer[i] = m_Buffer[i + 1] = (int32)sinVal & Mask;
		}
	}

private:
	uint16 m_SampleRate;
	float m_Frequency;
	float m_Amplitude;
	T *m_Buffer;
	uint16 m_BufferLength;
};
#endif