#pragma once
#ifndef BIQUAD_FILTER_H
#define BIQUAD_FILTER_H

#include "Filter.h"
#include "../Math.h"
#include "../Memory.h"

// https://en.wikipedia.org/wiki/Digital_biquad_filter#Direct_form_2
class BiquadFilter : public Filter
{
public:
	struct Coefficients
	{
		// The denominator (feedback part) of the filter transfer function.
		double a1;
		double a2;

		// The numerator (feedforward part) of the filter transfer function.
		double b0;
		double b1;
		double b2;
	};

private:
	struct Stage
	{
		Coefficients Coeffs;
		double w0;
		double w1;
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

		// for (uint8 i = 0; i < m_StageCount; ++i)
		// 	printf("Coeffs %i, a1: %f, a2: %f, b0: %f, b1: %f, b2: %f\n", i, Values[i].a1, Values[i].a2, Values[i].b0, Values[i].b1, Values[i].b2);
	}

	void Reset(void)
	{
		for (uint8 i = 0; i < m_StageCount; ++i)
		{
			Stage &stage = m_Stages[i];

			stage.w0 = 0;
			stage.w1 = 0;
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

			double temp = Value + stage.Coeffs.a1 * stage.w0 + stage.Coeffs.a2 * stage.w1;
			Value = temp * stage.Coeffs.b0 + stage.Coeffs.b1 * stage.w0 + stage.Coeffs.b2 * stage.w1;

			stage.w1 = stage.w0;
			stage.w0 = temp;
		}

		return Value;
	}

private:
	uint8 m_StageCount;
	Stage *m_Stages;

public:
	// Needs a 2nd Order BiquadFilter with 1 Stage
	// CenterFrequency [MIN_FREQUENCY, MAX_FREQUENCY]
	// Bandwidth [MIN_FREQUENCY, MAX_FREQUENCY]
	static void SetBandPassFilterCoefficients(BiquadFilter *Filter, float CenterFrequency, float Bandwidth)
	{
		ASSERT(Filter != nullptr, "Filter cannot be null");
		ASSERT(MIN_FREQUENCY <= CenterFrequency && CenterFrequency <= MAX_FREQUENCY, "Invalid CenterFrequency");
		ASSERT(MIN_FREQUENCY <= Bandwidth && Bandwidth <= MAX_FREQUENCY, "Invalid Bandwidth");

		double w0 = Math::TWO_PI_VALUE * CenterFrequency;

		double cosw0 = cosf(w0);
		double sinw0 = sinf(w0);

		double alpha = sinw0 / (Bandwidth / MAX_FREQUENCY);

		Coefficients coeffs;
		coeffs.a1 = (1 + alpha) / 2;
		coeffs.a2 = 0;
		coeffs.b0 = -(1 + alpha) / 2;
		coeffs.b1 = -2 * cosw0;
		coeffs.b2 = 1;

		coeffs.a1 /= coeffs.b0;
		coeffs.a2 /= coeffs.b0;
		coeffs.b0 /= coeffs.b0;
		coeffs.b1 /= coeffs.b0;
		coeffs.b2 /= coeffs.b0;

		Filter->SetCoefficients(&coeffs);
	}

	// Needs a 4th Order BiquadFilter with 2 Stages
	// SampleRate [MIN_SAMPLE_RATE, MAX_SAMPLE_RATE]
	// CutoffFrequency [MIN_FREQUENCY, MAX_FREQUENCY]
	static void SetNoiseGateCoefficients(BiquadFilter *Filter, uint32 SampleRate, float CutoffFrequency)
	{
		ASSERT(Filter != nullptr, "Filter cannot be null");
		ASSERT(MIN_SAMPLE_RATE <= SampleRate && SampleRate <= MAX_SAMPLE_RATE, "Invalid SampleRate");
		ASSERT(MIN_FREQUENCY <= CutoffFrequency && CutoffFrequency <= MAX_FREQUENCY, "Invalid CutoffFrequency");

		double w0 = Math::TWO_PI_VALUE * CutoffFrequency / SampleRate;

		double cosw0 = cosf(w0);
		double alpha = sinf(w0 / 2);

		Coefficients coeffs[2];
		{
			coeffs[0].a1 = -2 * cosw0;
			coeffs[0].a2 = 1 - alpha;
			coeffs[0].b0 = (1 - cosw0) / 2;
			coeffs[0].b1 = 1 - cosw0;
			coeffs[0].b2 = (1 - cosw0) / 2;
		}
		{
			coeffs[1].a1 = coeffs[0].a1;
			coeffs[1].a2 = coeffs[0].a2;
			coeffs[1].b0 = coeffs[0].b0;
			coeffs[1].b1 = -cosw0;
			coeffs[1].b2 = coeffs[0].b2;
		}

		// 4th order 20Hz lpf (44.1KHz)
		// {
		// coeffs[0].a1 = 1.9947405124091158;
		// coeffs[0].a2 = -0.9947486108316238;
		// coeffs[0].b0 = 0.000002152381733479521;
		// coeffs[0].b1 = 0.000004304763466959042;
		// coeffs[0].b2 = 0.000002152381733479521;

		// coeffs[1].a1 = 1.997813341671618;
		// coeffs[1].a2 = -0.9978214525694677;
		// coeffs[1].b0 = 0.0000019073486328125;
		// coeffs[1].b1 = 0.000003814697265625;
		// coeffs[1].b2 = 0.0000019073486328125;
		// }

		Filter->SetCoefficients(coeffs);
	}
};
#endif