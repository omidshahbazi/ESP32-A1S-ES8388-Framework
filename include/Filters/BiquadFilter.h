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
		float a1;
		float a2;

		// The numerator (feedforward part) of the filter transfer function.
		float b0;
		float b1;
		float b2;
	};

private:
	struct Stage
	{
		Coefficients Coeffs;
		float w0;
		float w1;
	};

public:
	BiquadFilter(uint8 StageCount)
		: m_StageCount(StageCount),
		  m_Stages(nullptr)
	{
		ASSERT(m_StageCount != 0, "BiquadFilter", "StageCount cannot be 0");

		m_Stages = Memory::Allocate<Stage>(m_StageCount);
	}

	~BiquadFilter(void)
	{
		Memory::Deallocate(m_Stages);
	}

	void SetCoefficients(const Coefficients *Values)
	{
		ASSERT(Values != nullptr, "BiquadFilter", "Values cannot be null");

		for (uint8 i = 0; i < m_StageCount; ++i)
		{
			m_Stages[i].Coeffs = Values[i];

			// printf("Coeffs %i, a1: %f, a2: %f, b0: %f, b1: %f, b2: %f\n",
			// 	   i, Values[i].a1, Values[i].a2, Values[i].b0, Values[i].b1, Values[i].b2);
		}
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
		for (uint8 i = 0; i < m_StageCount; ++i)
		{
			double input = Value;

			Stage &stage = m_Stages[i];

			float temp = Value + stage.Coeffs.a1 * stage.w0 + stage.Coeffs.a2 * stage.w1;
			Value = temp * stage.Coeffs.b0 + stage.Coeffs.b1 * stage.w0 + stage.Coeffs.b2 * stage.w1;

			stage.w1 = stage.w0;
			stage.w0 = temp;

			// printf("Values %i, temp: %f, in: %f, out: %f, w0: %f, w1: %f\n",
			// 	   i, temp, input, Value, stage.w0, stage.w1);
		}

		return Value;
	}

private:
	uint8 m_StageCount;
	Stage *m_Stages;
};
#endif