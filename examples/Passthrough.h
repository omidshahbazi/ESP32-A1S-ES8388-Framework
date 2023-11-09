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
		configs.TransmissionMode = ESP32A1SCodec::TransmissionModes::Both;
		configs.SampleRate = 44100;
		configs.BitsPerSample = ES8388::BitsPerSamples::BPS32;
		configs.ChannelFormat = ESP32A1SCodec::ChannelFormats::SeparatedLeftAndRight;
		configs.BufferCount = 3;
		configs.BufferLegth = 300;
		configs.InputMode = ES8388::InputModes::LeftAndRightInput1;
		configs.OutputMode = ES8388::OutputModes::AllLineOutputs;

		CHECK_CALL(ESP32A1SCodec::Initialize(&configs));

		Task::Create(OutputTask);
	}

private:
	static void OutputTask(void *args)
	{
		int32 *buffer = Memory::Allocate<int32>(FRAME_LENGTH);

		while (true)
		{
			ESP32A1SCodec::Read(buffer, FRAME_LENGTH, 20);

			ESP32A1SCodec::Write(buffer, FRAME_LENGTH, 20);
		}

		Memory::Deallocate(buffer);

		vTaskDelete(nullptr);
	}

private:
	static const uint16 FRAME_LENGTH;
	static const uint16 SAMPLE_RATE;
};

const uint16 Passthrough::FRAME_LENGTH = 64;
const uint16 Passthrough::SAMPLE_RATE = 44100;

#endif