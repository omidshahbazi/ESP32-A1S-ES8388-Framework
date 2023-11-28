#pragma once
#ifndef REVERB_H
#define REVERB_H

#include "IDSP.h"
#include "../Math.h"

class Reverb : public IDSP
{
public:
	Reverb(uint32 SampleRate)
	{
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
	}
};

#endif
