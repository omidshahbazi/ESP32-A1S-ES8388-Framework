#pragma once
#ifndef ESP32_A1S_CODEC_H
#define ESP32_A1S_CODEC_H

#include "ES8388.h"
#include "Memory.h"
#include <driver/i2s.h>
#include <driver/i2c.h>

class ESP32A1SCodec
{
public:
	enum class Versions
	{
		V2957 = 0,
		V2974
	};

	enum class BitsPerSamples
	{
		BPS16 = (uint8)ES8388::BitsPerSamples::BPS16,
		BPS24 = (uint8)ES8388::BitsPerSamples::BPS24,
		BPS32 = (uint8)ES8388::BitsPerSamples::BPS32
	};

	enum class ChannelFormats
	{
		LeftAndRight = I2S_CHANNEL_FMT_RIGHT_LEFT,

		AllFromLeft = I2S_CHANNEL_FMT_ALL_LEFT,
		AllFromRight = I2S_CHANNEL_FMT_ALL_RIGHT,

		OnlyLeft = I2S_CHANNEL_FMT_ONLY_LEFT,
		OnlyRight = I2S_CHANNEL_FMT_ONLY_RIGHT
	};

	// TODO: Renaming
	enum class InputModes
	{
		None = (uint8)ES8388::InputModes::None,

		Microphone1 = (uint8)ES8388::InputModes::Left1,
		Microphone2 = (uint8)ES8388::InputModes::Right1,

		LineL = (uint8)ES8388::InputModes::Left2,
		LineR = (uint8)ES8388::InputModes::Right2,
	};

	enum class OutputModes
	{
		None = (uint8)ES8388::OutputModes::None,

		SpeakerL = (uint8)ES8388::OutputModes::Left1,
		SpeakerR = (uint8)ES8388::OutputModes::Right1,

		HeadphoneL = (uint8)ES8388::OutputModes::Left2,
		HeadphoneR = (uint8)ES8388::OutputModes::Right2,

		SpeakerLAndHeadphoneL = (uint8)ES8388::OutputModes::Left1AndLeft2,
		SpeakerRAndHeadphoneR = (uint8)ES8388::OutputModes::Right1AndRight2,

		SpeakerLR = (uint8)ES8388::OutputModes::LeftAndRight1,
		HeadphoneLR = (uint8)ES8388::OutputModes::LeftAndRight2,

		All = (uint8)ES8388::OutputModes::All
	};

	struct Configs
	{
	public:
		Versions Version;
		uint32 SampleRate;
		BitsPerSamples BitsPerSample;
		ChannelFormats ChannelFormat;
		uint16 BufferCount;
		uint16 BufferLength;
		InputModes InputMode;
		OutputModes OutputMode;
	};

public:
	static void Initialize(Configs *Configs)
	{
		CHECK_CALL(InitializeI2C(Configs));

		m_Codec = new ES8388((ES8388::InputModes)Configs->InputMode, (ES8388::OutputModes)Configs->OutputMode);
		CHECK_CALL(m_Codec->SetBitsPerSample((ES8388::BitsPerSamples)Configs->BitsPerSample));

		CHECK_CALL(InitializeI2S(Configs));
	}

	//[0dB, 24dB]
	static void SetMicrophoneGain(float dB)
	{
		CHECK_CALL(m_Codec->SetMicrophoneGain(dB));
	}

	static float GetMicrophoneGain(void)
	{
		return m_Codec->GetMicrophoneGain();
	}

	//[-96dB, 0dB]
	static void SetInputVolume(float dB)
	{
		CHECK_CALL(m_Codec->SetInputVolume(dB));
	}

	static float GetInputVolume(void)
	{
		return m_Codec->GetInputVolume();
	}

	//[-45dB, 4.5dB]
	static void SetOutputVolume(float dB)
	{
		CHECK_CALL(m_Codec->SetOutputVolume(dB));
	}

	static float GetOutputVolume(void)
	{
		return m_Codec->GetOutputVolume();
	}

	static void SetMute(bool Mute)
	{
		CHECK_CALL(m_Codec->SetMute(Mute));
	}

	static bool GetMute(void)
	{
		return m_Codec->GetMute();
	}

	// Optimize the analog to digital conversion range
	//[0, 4]
	//(1Vrms/2.83Vpp, 0.5Vrms/1.41Vpp, 0.25Vrms/707mVpp, 0.125Vrms/354mVpp, 0.625Vrms/177mVpp)
	static void OptimizeConversion(uint8 Range = 2)
	{
		CHECK_CALL(m_Codec->OptimizeConversion(Range));
	}

	template <typename T>
	static void Read(T *Buffer, uint32 Count, int32 TicksToWait = -1)
	{
		uint32 readByteCount = 0;
		Read(Buffer, Count, &readByteCount, TicksToWait);
	}

	template <typename T>
	static void Read(T *Buffer, uint32 Count, uint32 *ReadByteCount, int32 TicksToWait = -1)
	{
		ReadRaw(Buffer, Count * sizeof(T), ReadByteCount, TicksToWait);
	}

	static void ReadRaw(void *Buffer, uint32 Length, uint32 *ReadByteCount, int32 TicksToWait = -1)
	{
		ESP_CHECK_CALL(i2s_read(I2S_PORT, Buffer, Length, ReadByteCount, TicksToWait));

		Log::WriteDebug(TAG, "Read %ib from the I2S in a %ib long buffer with %iticks for timeout", *ReadByteCount, Length, TicksToWait);
	}

	template <typename T>
	static void Write(const T *Buffer, uint32 Count, int32 TicksToWait = -1)
	{
		uint32 writtenByteCount = 0;
		Write(Buffer, Count, &writtenByteCount, TicksToWait);
	}

	template <typename T>
	static void Write(const T *Buffer, uint32 Count, uint32 *WrittenByteCount, int32 TicksToWait = -1)
	{
		WriteRaw(Buffer, Count * sizeof(T), WrittenByteCount, TicksToWait);
	}

	static void WriteRaw(const void *Buffer, uint32 Length, uint32 *WrittenByteCount, int32 TicksToWait = -1)
	{
		ESP_CHECK_CALL(i2s_write(I2S_PORT, Buffer, Length, WrittenByteCount, TicksToWait));

		Log::WriteDebug(TAG, "Wrote %ib to the I2S from a %ib long buffer with %iticks for timeout", *WrittenByteCount, Length, TicksToWait);
	}

private:
	static bool InitializeI2C(Configs *Configs)
	{
		Log::WriteInfo(TAG, "Initializing I2C");

		i2c_config_t config = {};
		config.mode = I2C_MODE_MASTER;
		config.sda_pullup_en = GPIO_PULLUP_ENABLE;
		config.scl_pullup_en = GPIO_PULLUP_ENABLE;
		config.master.clk_speed = 100 * KHz;

		if (Configs->Version == Versions::V2957)
		{
			config.sda_io_num = GPIO_NUM_33;
			config.scl_io_num = GPIO_NUM_32;
		}
		else if (Configs->Version == Versions::V2974)
		{
			config.sda_io_num = GPIO_NUM_18;
			config.scl_io_num = GPIO_NUM_23;
		}
		else
			return false;

		ESP_CHECK_CALL(i2c_param_config(I2C_PORT, &config));

		ESP_CHECK_CALL(i2c_driver_install(I2C_PORT, config.mode, 0, 0, 0));

		return true;
	}

	static bool InitializeI2S(Configs *Configs)
	{
		Configs->BufferLength = Math::Clamp(Configs->BufferLength, 1, 1024);

		Log::WriteInfo(TAG, "Initializing I2S");

		i2s_mode_t modes = I2S_MODE_MASTER;
		if (Configs->OutputMode != OutputModes::None)
			modes |= I2S_MODE_TX;
		if (Configs->InputMode != InputModes::None)
			modes |= I2S_MODE_RX;

		i2s_bits_per_sample_t bps = I2S_BITS_PER_SAMPLE_8BIT;
		switch (Configs->BitsPerSample)
		{
		case BitsPerSamples::BPS16:
			bps = I2S_BITS_PER_SAMPLE_16BIT;
			break;

		case BitsPerSamples::BPS24:
			bps = I2S_BITS_PER_SAMPLE_24BIT;
			break;

		case BitsPerSamples::BPS32:
			bps = I2S_BITS_PER_SAMPLE_32BIT;
			break;
		}

		i2s_config_t config = {};
		config.mode = modes;
		config.sample_rate = Configs->SampleRate;
		config.bits_per_sample = bps;
		config.channel_format = (i2s_channel_fmt_t)Configs->ChannelFormat;
		config.communication_format = I2S_COMM_FORMAT_STAND_I2S;
		config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
		config.dma_buf_count = Configs->BufferCount;
		config.dma_buf_len = Configs->BufferLength;
		config.use_apll = true;
		config.tx_desc_auto_clear = true;
		config.mclk_multiple = I2S_MCLK_MULTIPLE_DEFAULT;
		config.fixed_mclk = 33868800;

		ESP_CHECK_CALL(i2s_driver_install(I2S_PORT, &config, 0, nullptr));

		CHECK_CALL(SetI2SPin(Configs));

		i2s_channel_t channels = I2S_CHANNEL_STEREO;
		switch (Configs->ChannelFormat)
		{
		case ChannelFormats::OnlyLeft:
		case ChannelFormats::OnlyRight:
			channels = I2S_CHANNEL_MONO;
			break;
		}

		ESP_CHECK_CALL(i2s_set_clk(I2S_PORT, config.sample_rate, config.bits_per_sample, channels));

		return true;
	}

	static bool SetI2SPin(Configs *Configs)
	{
		i2s_pin_config_t config = {};
		config.ws_io_num = GPIO_NUM_25;
		config.data_out_num = GPIO_NUM_26;
		config.data_in_num = GPIO_NUM_35;

		gpio_num_t masterClockPin = GPIO_NUM_0;

		if (Configs->Version == Versions::V2957)
		{
			config.bck_io_num = GPIO_NUM_27;
			masterClockPin = GPIO_NUM_3;
		}
		else if (Configs->Version == Versions::V2974)
		{
			config.bck_io_num = GPIO_NUM_5;
			masterClockPin = GPIO_NUM_0;
		}
		else
		{
			Log::WriteError(TAG, "%i for version of the module is not supported", Configs->Version);
			return false;
		}

		Log::WriteInfo(TAG, "Setting the connection pins for I2S%i, WS: GPIO%i, DO: GPIO%i, DI: GPIO%i, BCK: GPIO%i", I2S_PORT, config.ws_io_num, config.data_out_num, config.data_in_num, config.bck_io_num);

		ESP_CHECK_CALL(i2s_set_pin(I2S_PORT, &config));

		SetMasterClockPin(I2S_PORT, masterClockPin);

		return true;
	}

	static void SetMasterClockPin(i2s_port_t Port, gpio_num_t GPIO)
	{
		ASSERT(Port != I2S_NUM_MAX, "Setting MasterClockPin", "Does not support I2S_NUM_MAX");
		ASSERT(GPIO == GPIO_NUM_0 || GPIO == GPIO_NUM_1 || GPIO == GPIO_NUM_3, "Setting MasterClockPin", "GPIO_NUM_0, GPIO_NUM_1 and GPIO_NUM_3 are only supported for master");

		Log::WriteInfo(TAG, "Setting the master clock for I2S%d on GPIO%d", Port, GPIO);

		if (Port == I2S_NUM_0)
		{
			if (GPIO == GPIO_NUM_0)
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
				// WRITE_PERI_REG(PIN_CTRL, 0xFFF0);
				WRITE_PERI_REG(PIN_CTRL, READ_PERI_REG(PIN_CTRL) & 0xFFFFFFF0);
			}
			else if (GPIO == GPIO_NUM_1)
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD_CLK_OUT3);
				WRITE_PERI_REG(PIN_CTRL, 0xF0F0);
			}
			else
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_U0RXD_CLK_OUT2);
				WRITE_PERI_REG(PIN_CTRL, 0xFF00);
			}
		}
		else if (Port == I2S_NUM_1)
		{
			if (GPIO == GPIO_NUM_0)
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
				// WRITE_PERI_REG(PIN_CTRL, 0xFFFF);
				WRITE_PERI_REG(PIN_CTRL, READ_PERI_REG(PIN_CTRL) & 0xFFFFFFFF);
			}
			else if (GPIO == GPIO_NUM_1)
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD_CLK_OUT3);
				WRITE_PERI_REG(PIN_CTRL, 0xF0FF);
			}
			else
			{
				PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_U0RXD_CLK_OUT2);
				WRITE_PERI_REG(PIN_CTRL, 0xFF0F);
			}
		}
	}

private:
	static ES8388 *m_Codec;

	static const char *TAG;
	static const i2c_port_t I2C_PORT;
	static const i2s_port_t I2S_PORT;
};

ES8388 *ESP32A1SCodec::m_Codec = nullptr;

const char *ESP32A1SCodec::TAG = "ESP32A1SCodec";
const i2c_port_t ESP32A1SCodec::I2C_PORT = I2C_NUM_0;
const i2s_port_t ESP32A1SCodec::I2S_PORT = I2S_NUM_0;

#endif