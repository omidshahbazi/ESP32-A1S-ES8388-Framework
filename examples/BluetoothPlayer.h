#pragma once
#ifndef BLUETOOTH_PLAYER_H
#define BLUETOOTH_PLAYER_H

#include "../include/ESP32A1SCodec.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_a2dp_api.h"

static const char *TAG = "BT_SINK";

class BluetoothPlayer
{
public:
	static void EntryPoint(void)
	{
		Log::SetMask(Log::Types::General);

		ESP32A1SCodec::Configs configs;
		configs.Version = ESP32A1SCodec::Versions::V2974;
		configs.SampleRate = SAMPLE_RATE;
		configs.BitsPerSample = ESP32A1SCodec::BitsPerSamples::BPS16;
		configs.ChannelFormat = ESP32A1SCodec::ChannelFormats::LeftAndRight;
		configs.BufferCount = 3;
		configs.BufferLength = 300;
		configs.InputMode = ESP32A1SCodec::InputModes::None;
		configs.OutputMode = ESP32A1SCodec::OutputModes::All;
		configs.MonoMixMode = ESP32A1SCodec::MonoMixModes::None;
		configs.EnableNoiseGate = false;
		configs.EnableAutomaticLevelControl = false;

		CHECK_CALL(ESP32A1SCodec::Initialize(&configs));

		esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
		esp_bt_controller_init(&bt_cfg);
		esp_bt_controller_enable(ESP_BT_MODE_BLE);
		esp_bluedroid_init();
		esp_bluedroid_enable();
		// esp_gap_register_callback(gap_event_handler);
		// esp_gatts_register_callback(gatts_event_handler);

		// Task::Create(OutputTask);
	}

private:
	// static void OutputTask(void *args)
	// {

	// 	while (true)
	// 	{

	// 		ESP32A1SCodec::Write(sineWave.GetBuffer(), sineWave.GetBufferLength());
	// 	}

	// 	vTaskDelete(nullptr);
	// }

private:
	static const uint16 SAMPLE_RATE;
};

const uint16 BluetoothPlayer::SAMPLE_RATE = 44100;

#endif