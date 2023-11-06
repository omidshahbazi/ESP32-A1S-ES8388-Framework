#pragma once
#ifndef PASSTHROUGH_H
#define PASSTHROUGH_H

#include "../include/ESP32A1SCodec.h"
#include "../include/SineWaveGenerator.h"

class Passthrough
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
	static void OutputTask(void *args)
	{
		SineWaveGenerator<int16> sineWave(SAMPLE_RATE);

		int amplitude = sineWave.GetAmplitude();
		int step = 25;

		while (true)
		{
			amplitude -= step;
			if (amplitude <= 50 || 15000 <= amplitude)
				step *= -1;

			sineWave.SetAmplitude(amplitude);

			ESP32A1SCodec::Write(sineWave.GetBuffer(), sineWave.GetBufferLength());
		}

		vTaskDelete(nullptr);
	}

private:
	static const uint16 SAMPLE_RATE;
};

const uint16 SineWavePlayer::SAMPLE_RATE = 44100;

#endif