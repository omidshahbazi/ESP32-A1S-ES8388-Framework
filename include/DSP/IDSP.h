#pragma once
#ifndef I_DSP_H
#define I_DSP_H

#include "../Common.h"

class IDSP
{
public:
	virtual void ProcessBuffer(double *Buffer, uint16 Count) = 0;
};

#endif