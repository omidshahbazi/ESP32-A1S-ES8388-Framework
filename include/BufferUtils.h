#pragma once
#ifndef BUFFER_UTILS_H
#define BUFFER_UTILS_H

#include "Math.h"

const int32 FULL_24_BITS = 0x7FFFFF;

#define CONVERT_TO_NORMALIZED_DOUBLE(SourceBuffer, SourceIsDoubleBuffered, SourceElementShiftBitCount, SourceBufferOffset, DestinationBuffer, Index) \
	{                                                                                                                                                \
		DestinationBuffer[Index] = SourceBuffer[(Index * (SourceIsDoubleBuffered ? 2 : 1)) + SourceBufferOffset] >> SourceElementShiftBitCount;      \
		DestinationBuffer[Index] /= FULL_24_BITS;                                                                                                    \
	}

#define CONVERT_INT16_TO_NORMALIZED_DOUBLE(SourceBuffer, SourceIsDoubleBuffered, SourceBufferOffset, DestinationBuffer, Index) \
	CONVERT_TO_NORMALIZED_DOUBLE(SourceBuffer, SourceIsDoubleBuffered, 0, SourceBufferOffset, DestinationBuffer, Index)

#define CONVERT_INT32_TO_NORMALIZED_DOUBLE(SourceBuffer, SourceIsDoubleBuffered, SourceBufferOffset, DestinationBuffer, Index) \
	CONVERT_TO_NORMALIZED_DOUBLE(SourceBuffer, SourceIsDoubleBuffered, 8, SourceBufferOffset, DestinationBuffer, Index)

#define SCALE_NORMALIZED_DOUBLE(SourceBuffer, Index, DestinationBuffer, DestinationIsDoubleBuffered, DestinationElementShiftBitCount, DestinationBufferOffset) \
	{                                                                                                                                                          \
		double process = SourceBuffer[Index] * FULL_24_BITS;                                                                                                   \
		process = Math::Clamp(process, -FULL_24_BITS, FULL_24_BITS);                                                                                           \
		DestinationBuffer[(Index * (DestinationIsDoubleBuffered ? 2 : 1)) + DestinationBufferOffset] = ((int32)process) << DestinationElementShiftBitCount;    \
	}

#define SCALE_NORMALIZED_DOUBLE_TO_INT16(SourceBuffer, Index, DestinationBuffer, DestinationIsDoubleBuffered, DestinationBufferOffset) \
	SCALE_NORMALIZED_DOUBLE(SourceBuffer, Index, DestinationBuffer, DestinationIsDoubleBuffered, 0, DestinationBufferOffset)

#define SCALE_NORMALIZED_DOUBLE_TO_INT32(SourceBuffer, Index, DestinationBuffer, DestinationIsDoubleBuffered, DestinationBufferOffset) \
	SCALE_NORMALIZED_DOUBLE(SourceBuffer, Index, DestinationBuffer, DestinationIsDoubleBuffered, 8, DestinationBufferOffset)

#endif