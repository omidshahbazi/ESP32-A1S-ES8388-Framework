#pragma once
#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "IDSP.h"

// TODO: Finalize this
class Compressor : public IDSP
{
private:
	double threshold; // Threshold for compression
	double ratio;	  // Compression ratio

public:
	Compressor(uint32 SampleRate, double Threshold = -0.9, double Ratio = 2.0)
		: threshold(Threshold), ratio(Ratio)
	{
		// Initialize your compressor with necessary parameters
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			// Apply compression to each sample in the buffer
			double input = Buffer[i];

			// Apply compression algorithm
			if (input > threshold)
			{
				double gainReduction = (input - threshold) / ratio;
				Buffer[i] = threshold + gainReduction;
			}
			// No compression applied if input is below the threshold

			// You can add makeup gain here if needed
		}
	}
};

#endif
