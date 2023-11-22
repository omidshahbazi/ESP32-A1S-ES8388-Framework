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
		configs.BitsPerSample = ESP32A1SCodec::BitsPerSamples::BPS16;
		configs.BufferCount = 3;
		configs.BufferLength = 300;
		configs.InputMode = ESP32A1SCodec::InputModes::None;
		configs.OutputMode = ESP32A1SCodec::OutputModes::All;
		configs.MonoMixMode = ESP32A1SCodec::MonoMixModes::None;
		configs.EnableNoiseGate = false;
		configs.EnableAutomaticLevelControl = false;

		CHECK_CALL(ESP32A1SCodec::Initialize(&configs));

		Task::Create(OutputTask);
	}

private:
	static void OutputTask(void *args)
	{
		SineWaveGenerator<int16> sineWave;
		sineWave.SetSampleRate(SAMPLE_RATE);

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

const uint16 SineWavePlayer::SAMPLE_RATE = SAMPLE_RATE_44100;

#endif