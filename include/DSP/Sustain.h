#if 1

#pragma once
#ifndef SUSTAIN_H
#define SUSTAIN_H

#include "IDSP.h"

class Sustain : public IDSP
{
public:
	Sustain(uint32 SampleRate)
	{
		// Initialize any parameters or variables here if needed
		sampleRate = SampleRate;
		feedbackFactor = 1;				   // Adjust as needed
		buffer = new double[sampleRate](); // MAX_BUFFER_SIZE should be defined appropriately
		bufferIndex = 0;
	}

	~Sustain()
	{
		delete[] buffer;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double inputSample = Buffer[i];

			// Apply sustain effect: mix input with delayed signal
			double delayedSample = buffer[(bufferIndex + sampleRate / 2) % sampleRate];
			double outputSample = inputSample + feedbackFactor * delayedSample;

			// Update buffer with the current input sample
			buffer[bufferIndex] = inputSample;
			bufferIndex = (bufferIndex + 1) % sampleRate;

			// Replace the input sample with the processed output sample
			Buffer[i] = outputSample;
		}
	}

private:
	uint32 sampleRate;
	double feedbackFactor;
	double *buffer;
	uint16 bufferIndex;
};

#endif

#else

#pragma once
#ifndef SUSTAIN_H
#define SUSTAIN_H

#include "IDSP.h"

class Sustain : public IDSP
{
public:
	Sustain(uint32 SampleRate)
	{
		sampleRate = SampleRate;
		feedbackFactor = 1; // Adjust as needed
		buffer = new double[sampleRate]();
		bufferIndex = 0;
		delaySamples = static_cast<uint16>(sampleRate * 0.1); // Adjust delay time (e.g., 0.1 seconds)
	}

	~Sustain()
	{
		delete[] buffer;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double inputSample = Buffer[i];

			// Apply sustain effect: mix input with delayed signal
			double delayedSample = buffer[(bufferIndex + delaySamples) % sampleRate];
			double outputSample = inputSample + feedbackFactor * delayedSample;

			// Update buffer with the current input sample
			buffer[bufferIndex] = outputSample; // Store the processed output for feedback
			bufferIndex = (bufferIndex + 1) % sampleRate;

			// Replace the input sample with the processed output sample
			Buffer[i] = outputSample;
		}
	}

private:
	uint32 sampleRate;
	double feedbackFactor;
	double *buffer;
	uint16 bufferIndex;
	uint16 delaySamples;
};

#endif

#endif