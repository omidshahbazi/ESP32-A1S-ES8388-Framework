#pragma once
#ifndef FILTER_H
#define FILTER_H

#include "../Common.h"

class Filter
{
public:
	virtual double Process(double Value) = 0;

	virtual void ProcessBuffer(double *Buffer, uint16 Count)
	{
		for (uint16 i = 0; i < Count; ++i)
			Buffer[i] = Process(Buffer[i]);
	}
};

#endif