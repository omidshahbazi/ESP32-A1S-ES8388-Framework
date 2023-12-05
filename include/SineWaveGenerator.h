#pragma once
#ifndef SINE_WAVE_GENERATOR_H
#define SINE_WAVE_GENERATOR_H

#include "Math.h"
#include "Notes.h"
#include "Memory.h"

template <typename T>
class SineWaveGenerator
{
	static_assert(std::is_same<T, int8>() || std::is_same<T, int16>() || std::is_same<T, int32>(), "T can only be either int8, int16 or int32");

public:
	SineWaveGenerator(void)
		: m_SampleRate(SAMPLE_RATE_44100),
		  m_Frequency(NOTE_G5),
		  m_Amplitude(8000),
		  m_DoubleBuffer(true),
		  m_Buffer(nullptr),
		  m_BufferLength(0)
	{
		SetupWave();
	}

	//[MIN_SAMPLE_RATE, MAX_SAMPLE_RATE]
	void SetSampleRate(uint16 Value)
	{
		ASSERT(MIN_SAMPLE_RATE <= Value && Value <= MAX_SAMPLE_RATE, "Invalid Value");

		m_SampleRate = Value;

		SetupWave();
	}
	uint16 GetSampleRate(void) const
	{
		return m_SampleRate;
	}

	//[MIN_FREQUENCY, MAX_FREQUENCY]
	void SetFrequency(float Value)
	{
		ASSERT(MIN_FREQUENCY <= Value && Value <= MAX_FREQUENCY, "Invalid Value");

		m_Frequency = Value;

		SetupWave();
	}
	float GetFrequency(void) const
	{
		return m_Frequency;
	}

	//[0, 1]
	void SetAmplitude(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		m_Amplitude = Value;

		SetupWave();
	}
	float GetAmplitude(void) const
	{
		return m_Amplitude;
	}

	void SetDoubleBuffered(bool Value)
	{
		m_DoubleBuffer = Value;

		SetupWave();
	}
	bool GetDoubleBuffered(void) const
	{
		return m_DoubleBuffer;
	}

	const T *GetBuffer(void) const
	{
		return m_Buffer;
	}
	uint32 GetBufferLength(void) const
	{
		return m_BufferLength;
	}

private:
	void SetupWave(void)
	{
		static const int32 MAX_VALUE = 0x7FFFFFFF >> (sizeof(int32) - sizeof(T));

		const uint16 SamplePerCycle = m_SampleRate / m_Frequency;

		const int8 STEP = (m_DoubleBuffer ? 2 : 1);
		uint32 requiredBufferLen = SamplePerCycle * STEP;

		if (requiredBufferLen != m_BufferLength)
		{
			if (m_Buffer != nullptr)
				Memory::Deallocate(m_Buffer);

			m_BufferLength = requiredBufferLen;
			m_Buffer = Memory::Allocate<T>(m_BufferLength);
		}

		for (uint32 i = 0; i < SamplePerCycle; ++i)
		{
			double sinVal = sin((i / (double)SamplePerCycle) * Math::TWO_PI_VALUE);

			uint32 index = i * STEP;

			m_Buffer[index] = sinVal * Math::Lerp(0.0, MAX_VALUE, m_Amplitude);

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
	uint32 m_BufferLength;
};
#endif