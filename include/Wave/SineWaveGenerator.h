#pragma once
#ifndef SINE_WAVE_GENERATOR_H
#define SINE_WAVE_GENERATOR_H

#include "../../include/ESP32A1SCodec.h"
#include "../Math.h"

template <typename T>
class SineWaveGenerator
{
public:
	SineWaveGenerator(uint16 SampleRate = 44100, float Frequency = 700, float Amplitude = 8000, bool DoubleBuffer = true)
		: m_SampleRate(1), m_Frequency(1), m_Amplitude(1), m_Buffer(nullptr), m_BufferLength(0)
	{
		SetSampleRate(SampleRate);
		SetFrequency(Frequency);
		SetFrequency(Amplitude);
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
			Memory::Deallocate(m_Buffer);

		const int8 STEP = (m_DoubleBuffer ? 2 : 1);

		m_BufferLength = SamplePerCycle * STEP;
		m_Buffer = Memory::Allocate<T>(m_BufferLength);

		const int32 Mask = 0x0FFFFFFF >> (sizeof(int32) - sizeof(T));

		for (uint32 i = 0; i < m_BufferLength; ++i)
		{
			double sinVal = sin(i / 2 * 2 * Math::PI / SamplePerCycle) * m_Amplitude;

			uint32 index = i * STEP;

			m_Buffer[index] = (int32)sinVal & Mask;

			if (m_DoubleBuffer)
				m_Buffer[index + 1] = m_Buffer[index];
		}
	}

private:
	uint16 m_SampleRate;
	float m_Frequency;
	float m_Amplitude;
	bool m_DoubleBuffer;
	T *m_Buffer;
	uint16 m_BufferLength;
};
#endif