#pragma once
#ifndef SINE_WAVE_PLAYER_H
#define SINE_WAVE_PLAYER_H

#include "../include/ESP32A1SCodec.h"
#include "../include/Wave/SineWaveGenerator.h"

class SineWavePlayer
{
public:
	static void EntryPoint(void)
	{
		Log::SetMask(Log::Types::General);

		ESP32A1SCodec::Configs configs;
		configs.Version = ESP32A1SCodec::Versions::V2974;
		configs.SampleRate = SAMPLE_RATE;
		configs.BitsPerSample = ES8388::BitsPerSamples::BPS16;
		configs.ChannelFormat = ESP32A1SCodec::ChannelFormats::SeparatedLeftAndRight;
		configs.BufferCount = 3;
		configs.BufferLegth = 300;
		configs.InputMode = ES8388::InputModes::None;
		configs.OutputMode = ES8388::OutputModes::AllLineOutputs;

		CHECK_CALL(ESP32A1SCodec::Initialize(&configs));

		Task::Create(OutputTask);
	}

private:
	static void OutputTask(void *args)
	{
		SineWaveGenerator<int16> sineWave(SAMPLE_RATE);

		int32 amplitude = sineWave.GetAmplitude();
		int32 step = 25;

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