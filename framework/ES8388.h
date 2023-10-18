#pragma once
#ifndef ES8388_H
#define ES8388_H

#include "Bitwise.h"
#include "Debug.h"
#include "ESPDebug.h"
#include "I2CUtils.h"
#include <driver/i2s.h>
#include <driver/i2c.h>

class ES8388
{
public:
	enum class Modules
	{
		ADC = 0x1,
		DAC = 0x2,
		Both = ADC | DAC,
		Line = 0x4
	};

	enum class BitsPerSamples
	{
		None = -1,
		BPS16 = 0x03,
		BPS24 = 0x00,
		BPS32 = 0x04,
	};

	enum class InputModes
	{
		None = -1,
		LeftAndRightInput1 = 0x00,
		Microphone1 = 0x05,
		Microphone2 = 0x06,
		LeftAndRightInput2 = 0x50,
		Difference = 0xf0
	};

	enum class OutputModes
	{
		None = -1,
		LeftOutput1 = 0x20,
		LeftOutput2 = 0x08,
		SpeakerOutput = 0x09,
		RightOutput1 = 0x10,
		RightOutput2 = 0x04,
		AllLineOutputs = LeftOutput1 | LeftOutput2 | RightOutput1 | RightOutput2
	};

	enum class Formats
	{
		Normal = 0,
		Left,
		Right,
		DSP,
	};

private:
	enum class ChipRegisters
	{
		Power = 0x02,
		Mode = 0x08,
		Control1 = 0x00,
		Control2 = 0x01
	};

	enum class ADCRegisters
	{
		Power = 0x03,
		Control1 = 0x09,
		Control2 = 0x0a,
		Control3 = 0x0b,
		Control4 = 0x0c,
		Control5 = 0x0d,
		Control6 = 0x0e,
		Control7 = 0x0f,
		Control8 = 0x10,
		Control9 = 0x11,
		Control10 = 0x12,
		Control11 = 0x13,
		Control12 = 0x14,
		Control13 = 0x15,
		Control14 = 0x16
	};

	enum class DACRegisters
	{
		Power = 0x04,
		Control1 = 0x17,
		Control2 = 0x18,
		Control3 = 0x19,
		Control4 = 0x1a,
		Control5 = 0x1b,
		Control6 = 0x1c,
		Control7 = 0x1d,
		Control8 = 0x1e,
		Control9 = 0x1f,
		Control10 = 0x20,
		Control11 = 0x21,
		Control12 = 0x22,
		Control13 = 0x23,
		Control14 = 0x24,
		Control15 = 0x25,
		Control16 = 0x26,
		Control17 = 0x27,
		Control18 = 0x28,
		Control19 = 0x29,
		Control20 = 0x2a,
		Control21 = 0x2b,
		Control22 = 0x2c,
		Control23 = 0x2d,
		Control24 = 0x2e,
		Control25 = 0x2f,
		Control26 = 0x30,
		Control27 = 0x31,
		Control28 = 0x32,
		Control29 = 0x33,
		Control30 = 0x34
	};

public:
	static bool Initialize(Modules Module, BitsPerSamples BitsPerSample, InputModes InputMode, OutputModes OutputMode, Formats Format)
	{
		CHECK_CALL(SetRegisters(InputMode, OutputMode));

		CHECK_CALL(ConfigI2S(BitsPerSample, Format));

		CHECK_CALL(SetVolume(70));

		CHECK_CALL(Start(Module));

		return true;
	}

	static bool SetVolume(int32 Volume)
	{
		if (Volume < 0)
			Volume = 0;
		else if (Volume > 100)
			Volume = 100;

		Volume /= 3;

		I2CWrite((uint8)DACRegisters::Control24, Volume);
		I2CWrite((uint8)DACRegisters::Control25, Volume);
		I2CWrite((uint8)DACRegisters::Control26, Volume);
		I2CWrite((uint8)DACRegisters::Control27, Volume);

		return true;
	}

	static bool SetMute(bool Enabled)
	{
		I2CWrite((uint8)DACRegisters::Control3, (I2CRead((uint8)DACRegisters::Control3) & 0xFB) | (((int32)Enabled) << 2));

		return true;
	}

private:
	static bool SetRegisters(InputModes InputMode, OutputModes OutputMode)
	{
		enum class MasterModes
		{
			None = -1,
			Slave = 0x00,
			Master = 0x01
		};

		I2CWrite((uint8)DACRegisters::Control3, 0x04); // 0x04 mute/0x00 unmute&ramp;DAC unmute and  disabled digital volume control soft ramp

		I2CWrite((uint8)ChipRegisters::Control2, 0x50);
		I2CWrite((uint8)ChipRegisters::Power, 0x00);					 // normal all and power up all
		I2CWrite((uint8)ChipRegisters::Mode, (uint8)MasterModes::Slave); // CODEC IN I2S SLAVE MODE

		I2CWrite((uint8)DACRegisters::Power, 0xC0);		// disable DAC and disable Lout/Rout/1/2
		I2CWrite((uint8)ChipRegisters::Control1, 0x12); // Enfr=0,Play&Record Mode,(0x17-both of mic&paly)
		I2CWrite((uint8)DACRegisters::Control1, 0x18);	// 1a 0x18:16bit iis , 0x00:24
		I2CWrite((uint8)DACRegisters::Control2, 0x02);	// DACFsMode,SINGLE SPEED; DACFsRatio,256
		I2CWrite((uint8)DACRegisters::Control16, 0x00); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2
		I2CWrite((uint8)DACRegisters::Control17, 0x90); // only left DAC to left mixer enable 0db
		I2CWrite((uint8)DACRegisters::Control20, 0x90); // only right DAC to right mixer enable 0db
		I2CWrite((uint8)DACRegisters::Control21, 0x80); // set internal ADC and DAC use the same LRCK clock, ADC LRCK as internal LRCK
		I2CWrite((uint8)DACRegisters::Control23, 0x00); // vroi=0

		SetDACVolume(0, 0); // 0db

		LOG_INFO(FRAMEWORK_TAG, "Setting DAC Output: %02x", OutputMode);
		I2CWrite((uint8)DACRegisters::Power, (uint8)OutputMode);

		LOG_INFO(FRAMEWORK_TAG, "Setting ADC Input: %02x", InputMode);
		I2CWrite((uint8)ADCRegisters::Power, 0xFF);
		I2CWrite((uint8)ADCRegisters::Control1, 0xbb); // MIC Left and Right channel PGA gain
		I2CWrite((uint8)ADCRegisters::Control2, (uint8)InputMode);

		I2CWrite((uint8)ADCRegisters::Control3, 0x02);
		I2CWrite((uint8)ADCRegisters::Control4, 0x0d); // Left/Right data, Left/Right justified mode, Bits length, I2S format
		I2CWrite((uint8)ADCRegisters::Control5, 0x02); // ADCFsMode,singel SPEED,RATIO=256

		// ALC for Microphone
		SetADCVolume(0, 0);							// 0db
		I2CWrite((uint8)ADCRegisters::Power, 0x09); // Power on ADC, Enable LIN&RIN, Power off MICBIAS, set int1lp to low power mode

		return true;
	}

	static bool ConfigI2S(BitsPerSamples BitsPerSample, Formats Format) // Modules Module,
	{
		LOG_INFO(FRAMEWORK_TAG, "Setting I2S ADC Format: %x", Format);
		I2CWrite((uint8)ADCRegisters::Control4, (I2CRead((uint8)ADCRegisters::Control4) & 0xfc) | (uint8)Format);

		LOG_INFO(FRAMEWORK_TAG, "Setting I2S DAC Format: %x", Format);
		I2CWrite((uint8)DACRegisters::Control1, (I2CRead((uint8)DACRegisters::Control1) & 0xf9) | ((uint8)Format << 1));

		LOG_INFO(FRAMEWORK_TAG, "Setting I2S ADC Bits: %x", BitsPerSample);
		I2CWrite((uint8)ADCRegisters::Control4, (I2CRead((uint8)ADCRegisters::Control4) & 0xe3) | ((int32)BitsPerSample << 2));

		LOG_INFO(FRAMEWORK_TAG, "Setting I2S DAC Bits: %x", BitsPerSample);
		I2CWrite((uint8)DACRegisters::Control1, (I2CRead((uint8)DACRegisters::Control1) & 0xc7) | ((int32)BitsPerSample << 3));

		return true;
	}

	static bool Start(Modules Module)
	{
		uint8 prevData = I2CRead((uint8)DACRegisters::Control21);

		if (Bitwise::IsEnabled(Module, Modules::Line))
		{
			I2CWrite((uint8)DACRegisters::Control16, 0x09); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2 by pass enable
			I2CWrite((uint8)DACRegisters::Control17, 0x50); // left DAC to left mixer enable  and  LIN signal to left mixer enable 0db  : bupass enable
			I2CWrite((uint8)DACRegisters::Control20, 0x50); // right DAC to right mixer enable  and  LIN signal to right mixer enable 0db : bupass enable
			I2CWrite((uint8)DACRegisters::Control21, 0xC0); // enable adc
		}
		else
		{
			I2CWrite((uint8)DACRegisters::Control21, 0x80); // enable dac
		}

		uint8 data = I2CRead((uint8)DACRegisters::Control21);

		if (prevData != data)
		{
			LOG_INFO(FRAMEWORK_TAG, "Resetting State Machine");

			I2CWrite((uint8)ChipRegisters::Power, 0xF0); // start state machine
			// I2CWrite((uint8)ChipRegisters::Control1, 0x16);
			// I2CWrite((uint8)ChipRegisters::Control2, 0x50);
			// I2CWrite((uint8)ChipRegisters::Control2, 0x50);
			I2CWrite((uint8)ChipRegisters::Power, 0x00); // start state machine
		}

		if (Bitwise::IsEnabled(Module, Modules::ADC) || Bitwise::IsEnabled(Module, Modules::Line))
		{
			LOG_INFO(FRAMEWORK_TAG, "Powering up ADC");

			I2CWrite((uint8)ADCRegisters::Power, 0x00);
		}

		if (Bitwise::IsEnabled(Module, Modules::DAC) || Bitwise::IsEnabled(Module, Modules::Line))
		{
			LOG_INFO(FRAMEWORK_TAG, "Powering up DAC");

			I2CWrite((uint8)DACRegisters::Power, 0x3c);

			CHECK_CALL(SetMute(false));
		}

		return true;
	}

	static bool SetADCVolume(int32 Volume, int32 Dot)
	{
		int32 vol = GetVolume(Volume, Dot);

		I2CWrite((uint8)ADCRegisters::Control8, vol);
		I2CWrite((uint8)ADCRegisters::Control9, vol); // ADC Right Volume=0db

		return true;
	}

	static bool SetDACVolume(int32 Volume, int32 Dot)
	{
		int32 vol = GetVolume(Volume, Dot);

		I2CWrite((uint8)DACRegisters::Control5, vol);
		I2CWrite((uint8)DACRegisters::Control4, vol);

		return true;
	}

	static int32 GetVolume(int32 Volume, int32 Dot)
	{
		if (Volume < -96 || Volume > 0)
		{
			LOG_WARNING(FRAMEWORK_TAG, "Warning: volume < -96! or > 0!");

			if (Volume < -96)
				Volume = -96;
			else
				Volume = 0;
		}

		Dot = (Dot >= 5 ? 1 : 0);

		Volume = (-Volume << 1) + Dot;

		return Volume;
	}

	static uint8 I2CRead(uint8 Register)
	{
		return I2CUtils::Read(ADDRESS, Register);
	}

	static void I2CWrite(uint8 Register, uint8 Value)
	{
		CHECK_CALL(I2CUtils::Write(ADDRESS, Register, Value));
	}

private:
	static const int32 ADDRESS;
};

const int32 ES8388::ADDRESS = 0b0010000;

#endif