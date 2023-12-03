#pragma once
#ifndef PASSTHROUGH_H
#define PASSTHROUGH_H

#include "../include/ESP32A1SCodec.h"

class Passthrough
{
public:
	static void EntryPoint(void)
	{
		Log::SetMask(Log::Types::General);

		ESP32A1SCodec::Configs configs;
		configs.Version = ESP32A1SCodec::Versions::V2974;
		configs.SampleRate = SAMPLE_RATE_44100;
		configs.BitsPerSample = ESP32A1SCodec::BitsPerSamples::BPS32;
		configs.ChannelFormat = ESP32A1SCodec::ChannelFormats::LeftAndRight;
		configs.BufferCount = 3;
		configs.BufferLength = 300;
		configs.InputMode = ESP32A1SCodec::InputModes::Microphone1AndMicrophone2;
		configs.OutputMode = ESP32A1SCodec::OutputModes::All;
		configs.MonoMixMode = ESP32A1SCodec::MonoMixModes::None;
		configs.EnableNoiseGate = false;
		configs.EnableAutomaticLevelControl = false;

		ESP32A1SCodec::Initialize(&configs);

		Task::Create(OutputTask, 4096, "OutputTask", 1, 20);
	}

private:
	static void OutputTask(void)
	{
		int32 *buffer = Memory::Allocate<int32>(FRAME_LENGTH);

		while (true)
		{
			ESP32A1SCodec::Read(buffer, FRAME_LENGTH, 20);

			ESP32A1SCodec::Write(buffer, FRAME_LENGTH, 20);
		}

		Memory::Deallocate(buffer);

		Task::Delete();
	}

private:
	static const uint16 FRAME_LENGTH;
	static const uint16 SAMPLE_RATE;
};

const uint16 Passthrough::FRAME_LENGTH = 64;
const uint16 Passthrough::SAMPLE_RATE = SAMPLE_RATE_44100;

#endif