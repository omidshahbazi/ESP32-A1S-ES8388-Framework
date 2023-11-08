#pragma once
#ifndef WAVE_SHAPER_H
#define WAVE_SHAPER_H

#include "Wave.h"
#include "Tables.h"
#include "../Math.h"
#include <math.h>

class WaveShaper : public Wave
{
public:
	WaveShaper(void)
	{
		for (uint8 i = 0; i < TABLE_SIZE; ++i)
		{
			float x = ((float)i - 127.5f) / 127.5f;

			m_TransferFunctionTable[i] = 2.0f / (1 + expf(-6.0f * x)) - 1.0f;
		}
	}

	float Process(float Value) override
	{
		float val = (Value + 1.0f) * 127.5f;
		val = Math::Clamp(val, 0.0f, 255.0f);

		return LookupLinear(m_TransferFunctionTable, val);
	}

private:
	float m_TransferFunctionTable[TABLE_SIZE];
};
#endif