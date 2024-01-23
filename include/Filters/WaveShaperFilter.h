#pragma once
#ifndef WAVE_SHAPER_FILTER_H
#define WAVE_SHAPER_FILTER_H

#include "Filter.h"
#include "../Math.h"

class WaveShaperFilter : public Filter
{
public:
	struct TablePoints
	{
	public:
		float InputValue;
		float OutputValue;
	};

public:
	WaveShaperFilter(void)
		: m_Table(nullptr),
		  m_Length(0)
	{
	}

	void SetTable(const TablePoints *Table, uint8 Length)
	{
		ASSERT(Table != nullptr, "Invalid Table");
		ASSERT(Length > 1, "Invalid Length");

		m_Table = Table;
		m_Length = Length;
	}
	const TablePoints *GetTable(void) const
	{
		return m_Table;
	}
	uint8 GetLength(void) const
	{
		return m_Length;
	}

	double Process(double Value) override
	{
		const TablePoints *prev = nullptr;
		const TablePoints *next = nullptr;
		for (uint32 i = 0; i < m_Length; ++i)
		{
			const TablePoints &curr = m_Table[i];

			if (Value > curr.InputValue)
			{
				prev = &curr;

				continue;
			}

			next = &curr;
			break;
		}

		if (prev == nullptr)
		{
			prev = &m_Table[0];
		}

		if (next == nullptr)
		{
			next = &m_Table[m_Length - 1];
		}

		float pointsDiff = next->InputValue - prev->InputValue;
		float frac = (pointsDiff == 0 ? 0 : (Value - prev->InputValue) / pointsDiff);

		return Math::Lerp(prev->OutputValue, next->OutputValue, frac);
	}

private:
	const TablePoints *m_Table;
	uint8 m_Length;
};
#endif