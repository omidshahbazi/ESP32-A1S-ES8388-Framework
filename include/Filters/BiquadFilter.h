#pragma once
#ifndef BIQUAD_FILTER_H
#define BIQUAD_FILTER_H

#include "Filter.h"
#include "../Math.h"
#include "../Memory.h"
#include "../Debug.h"

// https://en.wikipedia.org/wiki/Digital_biquad_filter#Direct_form_2
// https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
// https://music-dsp.music.columbia.narkive.com/YF4UlHAc/auto-wah-implementation#post6
class BiquadFilter : public Filter
{
public:
	struct Coefficients
	{
		double a0; // Feedback (denominator) coefficient (pole)
		double a1; // Feedback (denominator) coefficient (pole)
		double a2; // Feedback (denominator) coefficient (pole)
		double b1; // Feedforward (numerator) coefficient (zero)
		double b2; // Feedforward (numerator) coefficient (zero)
	};

private:
	struct Parameters
	{
		float quality;
		float normalizedFrequency;
		float sqrNormalizedFrequency;
		float normalized;
	};

	struct Stage
	{
		Coefficients Coeffs;
		double z1;
		double z2;
	};

public:
	BiquadFilter(uint8 StageCount)
		: m_StageCount(StageCount),
		  m_Stages(nullptr)
	{
		ASSERT(m_StageCount != 0, "StageCount cannot be 0");

		m_Stages = Memory::Allocate<Stage>(m_StageCount);
	}

	~BiquadFilter(void)
	{
		Memory::Deallocate(m_Stages);
	}

	void SetCoefficients(const Coefficients *Values)
	{
		ASSERT(Values != nullptr, "Values cannot be null");

		for (uint8 i = 0; i < m_StageCount; ++i)
			m_Stages[i].Coeffs = Values[i];
	}

	void Reset(void)
	{
		for (uint8 i = 0; i < m_StageCount; ++i)
		{
			Stage &stage = m_Stages[i];

			stage.z1 = 0;
			stage.z2 = 0;
		}
	}

	double Process(double Value) override
	{
		if (m_Stages == nullptr)
			return Value;

		for (uint8 i = 0; i < m_StageCount; ++i)
		{
			double input = Value;

			Stage &stage = m_Stages[i];

			Value = (input * stage.Coeffs.a0) + stage.z1;
			stage.z1 = (input * stage.Coeffs.a1) + stage.z2 - (stage.Coeffs.b1 * Value);
			stage.z2 = (input * stage.Coeffs.a2) - (stage.Coeffs.b2 * Value);
		}

		return Value;
	}

private:
	static Parameters GetParameters(uint32 SampleRate, float Frequency, float Bandwidth, float Resonance)
	{
		ASSERT(1 <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");
		ASSERT(0 < Frequency && Frequency <= MAX_FREQUENCY, "Invalid Frequency");
		ASSERT(0 <= Bandwidth && Bandwidth <= MAX_FREQUENCY, "Invalid Bandwidth");

		Parameters params;

		params.quality = Resonance * Frequency / Math::Max(0.0001, Bandwidth);
		params.normalizedFrequency = tan(Math::PI_VALUE * Frequency / SampleRate);
		params.sqrNormalizedFrequency = params.normalizedFrequency * params.normalizedFrequency;
		params.normalized = 1 / (1 + (params.normalizedFrequency / params.quality) + params.sqrNormalizedFrequency);

		return params;
	}

public:
	// Needs a 1 stage BiquadFilter
	// SampleRate [1, MAX_SAMPLE_RATE]
	// CutoffFrequency [1, MAX_FREQUENCY]
	static void SetLowPassFilterCoefficients(BiquadFilter *Filter, uint32 SampleRate, float CutoffFrequency, float Resonance = 1)
	{
		ASSERT(Filter != nullptr, "Filter cannot be null");

		const Parameters params = GetParameters(SampleRate, CutoffFrequency, CutoffFrequency, Resonance);

		Coefficients coeffs;
		coeffs.a0 = params.sqrNormalizedFrequency * params.normalized;
		coeffs.a1 = 2 * coeffs.a0;
		coeffs.a2 = coeffs.a0;
		coeffs.b1 = 2 * (params.sqrNormalizedFrequency - 1) * params.normalized;
		coeffs.b2 = (1 - (params.normalizedFrequency / params.quality) + params.sqrNormalizedFrequency) * params.normalized;

		Filter->SetCoefficients(&coeffs);
	}

	// Needs a 1 stage BiquadFilter
	// SampleRate [1, MAX_SAMPLE_RATE]
	// CutoffFrequency [1, MAX_FREQUENCY]
	static void SetHighPassFilterCoefficients(BiquadFilter *Filter, uint32 SampleRate, float CutoffFrequency, float Resonance = 1)
	{
		ASSERT(Filter != nullptr, "Filter cannot be null");

		const Parameters params = GetParameters(SampleRate, CutoffFrequency, MAX_FREQUENCY - CutoffFrequency, Resonance);

		Coefficients coeffs;
		coeffs.a0 = params.normalized;
		coeffs.a1 = -2 * coeffs.a0;
		coeffs.a2 = coeffs.a0;
		coeffs.b1 = 2 * (params.sqrNormalizedFrequency - 1) * params.normalized;
		coeffs.b2 = (1 - (params.normalizedFrequency / params.quality) + params.sqrNormalizedFrequency) * params.normalized;

		Filter->SetCoefficients(&coeffs);
	}

	// Needs a 1 stage BiquadFilter
	// SampleRate [1, MAX_SAMPLE_RATE]
	// CenterFrequency [1, MAX_FREQUENCY]
	// Bandwidth [1, MAX_FREQUENCY]
	static void SetBandPassFilterCoefficients(BiquadFilter *Filter, uint32 SampleRate, float CenterFrequency, float Bandwidth, float Resonance = 1)
	{
		ASSERT(Filter != nullptr, "Filter cannot be null");

		const Parameters params = GetParameters(SampleRate, CenterFrequency, Bandwidth, Resonance);

		Coefficients coeffs;
		coeffs.a0 = (params.normalizedFrequency / params.quality) * params.normalized;
		coeffs.a1 = 0;
		coeffs.a2 = -coeffs.a0;
		coeffs.b1 = 2 * (params.sqrNormalizedFrequency - 1) * params.normalized;
		coeffs.b2 = (1 - (params.normalizedFrequency / params.quality) + params.sqrNormalizedFrequency) * params.normalized;

		Filter->SetCoefficients(&coeffs);
	}

	// Needs a 1 stage BiquadFilter
	// SampleRate [1, MAX_SAMPLE_RATE]
	// CenterFrequency [1, MAX_FREQUENCY]
	// Bandwidth [1, MAX_FREQUENCY]
	static void SetBandStopFilterCoefficients(BiquadFilter *Filter, uint32 SampleRate, float CenterFrequency, float Bandwidth, float Resonance = 1)
	{
		const Parameters params = GetParameters(SampleRate, CenterFrequency, Bandwidth, Resonance);

		Coefficients coeffs;
		coeffs.a0 = (1 + params.sqrNormalizedFrequency) * params.normalized;
		coeffs.a1 = 2 * (params.sqrNormalizedFrequency - 1) * params.normalized;
		coeffs.a2 = coeffs.a0;
		coeffs.b1 = coeffs.a1;
		coeffs.b2 = (1 - (params.normalizedFrequency / params.quality) + params.sqrNormalizedFrequency) * params.normalized;

		Filter->SetCoefficients(&coeffs);
	}

private:
	uint8 m_StageCount;
	Stage *m_Stages;
};
#endif