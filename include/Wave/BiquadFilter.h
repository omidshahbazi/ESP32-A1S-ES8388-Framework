#pragma once
#ifndef BIQUAD_FILTER_H
#define BIQUAD_FILTER_H

#include "Wave.h"
#include "../Math.h"
#include "../Memory.h"

class BiquadFilter : public Wave
{
private:
	struct State
	{
		float Coef[5]; // a1, a2, b0, b1, b2
		float W[2];
	};

public:
	BiquadFilter(uint8 StateCount)
		: m_States(nullptr),
		  m_StateCount(StateCount)
	{
		m_States = Memory::Allocate<State>(m_StateCount);
	}

	~BiquadFilter(void)
	{
		Memory::Deallocate(m_States);
	}

	void Reset(void)
	{
		Memory::Set(m_States, 0, m_StateCount);
	}

	void SetCoef(const float *Values)
	{
		for (uint8 i = 0; i < m_StateCount; ++i)
			for (uint8 j = 0; j < 5; ++j)
				m_States[i].Coef[j] = Values[(5 * i) + j];
	}

	double Process(double Value) override
	{
		for (uint8 i = 0; i < m_StateCount; ++i)
		{
			State state = m_States[i];

			float temp = Value + state.Coef[3] * state.W[0] + state.Coef[4] * state.W[1];
			Value = temp * state.Coef[0] + state.Coef[1] * state.W[0] + state.Coef[2] * state.W[1];
			state.W[1] = state.W[0];
			state.W[0] = temp;
		}

		return Value;
	}

private:
	State *m_States;
	uint8 m_StateCount;
};
#endif