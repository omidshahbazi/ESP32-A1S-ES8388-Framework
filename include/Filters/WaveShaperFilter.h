#pragma once
#ifndef WAVE_SHAPER_FILTER_H
#define WAVE_SHAPER_FILTER_H

#include "Filter.h"
#include "../Tables.h"
#include "../Math.h"

class WaveShaperFilter : public Filter
{
public:
	WaveShaperFilter(void)
	{
		for (uint16 i = 0; i < TABLE_SIZE; ++i)
		{
			float x = ((float)i - 127.5f) / 127.5f;

			m_TransferFunctionTable[i] = 2 / (1 + expf(-6 * x)) - 1;
		}
	}

	double Process(double Value) override
	{
		double val = (Value + 1) * 127.5f;
		val = Math::Clamp(val, 0, 255);

		return Math::TableLookupLinear(m_TransferFunctionTable, val);
	}

private:
	float m_TransferFunctionTable[TABLE_SIZE];
};
#endif