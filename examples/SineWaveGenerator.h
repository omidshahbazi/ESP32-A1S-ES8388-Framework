#pragma once
#ifndef SINE_WAVE_GENERATOR_H
#define SINE_WAVE_GENERATOR_H

#include "../include/ESP32A1SCodec.h"
#include <math.h>

#define FRAME_LENGTH 64
#define SAMPLE_COUNT FRAME_LENGTH / 2
#define FRAME_SIZE FRAME_LENGTH * 4

#define SAMPLE_RATE (44100)
#define WAVE_FREQ_HZ (700)
#define PI (3.14159265)

#define SAMPLE_PER_CYCLE (SAMPLE_RATE / WAVE_FREQ_HZ)

#define SAMPLES SAMPLE_PER_CYCLE // Total number of samples left and right
#define BUF_SAMPLES SAMPLES * 4	 // Size of DMA tx/rx buffer samples * left/right * 2 for 32 bit samples

class SineWaveGenerator
{
public:
	static void EntryPoint(void)
	{
		Log::SetMask(Log::Types::General);

		ESP32A1SCodec::Configs configs;
		configs.Version = ESP32A1SCodec::Versions::V2974;
		configs.TransmissionMode = ESP32A1SCodec::TransmissionModes::Transmit;
		configs.SampleRate = SAMPLE_RATE;
		configs.BitsPerSample = ES8388::BitsPerSamples::BPS16;
		configs.ChannelFormat = ESP32A1SCodec::ChannelFormats::SeparatedLeftAndRight;
		configs.BufferCount = 3;
		configs.BufferLegth = 300;
		configs.InputMode = ES8388::InputModes::None;
		configs.OutputMode = ES8388::OutputModes::AllLineOutputs;

		CHECK_CALL(ESP32A1SCodec::Initialize(&configs));

		xTaskCreatePinnedToCore(OutputTask, "SineWaveGeneratorTask", 4096, nullptr, 10, nullptr, 1);
	}

private:
	static void SetupSineWave16Bits(int32 Amplitude, int16 *Buffer)
	{
		for (int32 i = 0; i < BUF_SAMPLES; i += 2)
		{
			double sinVal = sin(i / 2 * 2 * PI / SAMPLE_PER_CYCLE) * Amplitude;

			Buffer[i] = (int32)sinVal & 0xFFFF;
			Buffer[i + 1] = (int32)sinVal & 0xFFFF;
		}
	}

	static void OutputTask(void *args)
	{
		int16 buffer[BUF_SAMPLES];

		int amplitude = 8000;
		int start_dir = 50;
		int dir = start_dir;

		while (true)
		{
			amplitude -= dir;
			if (amplitude <= start_dir || amplitude >= 15000)
				dir *= -1;

			SetupSineWave16Bits(amplitude, buffer);

			ESP32A1SCodec::Write(buffer, BUF_SAMPLES);
		}

		vTaskDelete(nullptr);
	}
};
#endif