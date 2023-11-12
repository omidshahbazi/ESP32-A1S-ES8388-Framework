#pragma once
#ifndef BUFFER_UTILS_H
#define BUFFER_UTILS_H

#include "Math.h"

const int32 FULL_24_BITS = 0x7FFFFF;

// Convert to 24 bit int then to float and scale to 1.0
#define CONVERT_TO_24_AND_NORMALIZED_DOUBLE(DoubleBuffer, DoubleBufferIndex, Int32Buffer, Int32BufferIndexOffset)       \
	{                                                                                                                   \
		DoubleBuffer[DoubleBufferIndex] = (double)(Int32Buffer[(DoubleBufferIndex * 2) + Int32BufferIndexOffset] >> 8); \
		DoubleBuffer[DoubleBufferIndex] /= FULL_24_BITS;                                                                \
	}

// Scale to 24 bit range and saturated 32
#define SCALE_TO_24_AND_SATURATED_32(DoubleBuffer, DoubleBufferIndex, Int32Buffer, Int32BufferIndexOffset) \
	{                                                                                                      \
		double process = DoubleBuffer[DoubleBufferIndex] * FULL_24_BITS;                                   \
		process = Math::Clamp(process, -FULL_24_BITS, FULL_24_BITS);                                       \
		Int32Buffer[(DoubleBufferIndex * 2) + Int32BufferIndexOffset] = ((int32)process) << 8;             \
	}

#endif