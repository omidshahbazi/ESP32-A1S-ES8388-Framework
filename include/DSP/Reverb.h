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
		// Initialize your reverb parameters and buffers here
		this->SampleRate = SampleRate;

		// Example: Set initial parameters
		delayTime = 0.5; // in seconds
		feedback = 0.5;

		// Calculate the number of samples for the delay line
		delaySamples = static_cast<int>(delayTime * SampleRate);

		// Initialize delay line buffer with zeros
		delayBuffer = new double[delaySamples];
		for (int i = 0; i < delaySamples; ++i)
		{
			delayBuffer[i] = 0.0;
		}
	}

	~Reverb()
	{
		// Clean up allocated memory
		delete[] delayBuffer;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		// Apply reverb processing to the input buffer
		for (uint16 i = 0; i < Count; ++i)
		{
			// Apply reverb algorithm (e.g., simple feedback delay)
			double delayedSample = delayBuffer[readIndex];
			Buffer[i] += feedback * delayedSample;

			// Update delay buffer (feedback)
			delayBuffer[writeIndex] = Buffer[i];

			// Increment read and write indices, handling circular buffer
			readIndex = (readIndex + 1) % delaySamples;
			writeIndex = (writeIndex + 1) % delaySamples;
		}
	}

private:
	uint32 SampleRate;
	double delayTime; // Reverb delay time in seconds
	double feedback;  // Feedback amount for the delay line

	int delaySamples;	 // Number of samples in the delay line
	double *delayBuffer; // Circular buffer for the delay line

	int readIndex = 0;	// Read index for the circular buffer
	int writeIndex = 0; // Write index for the circular buffer
};

#endif
