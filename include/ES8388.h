#pragma once
#ifndef ES8388_H
#define ES8388_H

#include "Bitwise.h"
#include "Debug.h"
#include "ESPDebug.h"
#include "I2CUtils.h"
#include "ES8388Interface.h"
#include <driver/i2s.h>
#include <driver/i2c.h>

class ES8388
{
public:
	enum class Modules
	{
		ADC = 0x1,
		DAC = 0x2,
		Both = ADC | DAC
	};

	enum class BitsPerSamples
	{
		BPS16 = (uint8)ES8388Interface::BitsPerSamples::BPS16,
		BPS24 = (uint8)ES8388Interface::BitsPerSamples::BPS24,
		BPS32 = (uint8)ES8388Interface::BitsPerSamples::BPS32
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

public:
	ES8388(Modules Modules, InputModes InputMode, OutputModes OutputMode)
		: m_Modules(Modules)
	{
		Log::WriteInfo(TAG, "Intializing");

		CHECK_CALL(ES8388Interface::TurnOn(false, ES8388Interface::MiddleVoltageResistances::R500K));

		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
			CHECK_CALL(ES8388Interface::SetADCPowered(true, false, ES8388Interface::IOModes::All));

		if (Bitwise::IsEnabled(m_Modules, Modules::DAC))
			CHECK_CALL(ES8388Interface::SetDACPowered(true, ES8388Interface::IOModes::All, ES8388Interface::OutputResistances::R1K5));

		CHECK_CALL(SetBitsPerSample(BitsPerSamples::BPS24));

		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
			CHECK_CALL(ES8388Interface::SetADCFormat(ES8388Interface::Formats::I2S));

		if (Bitwise::IsEnabled(m_Modules, Modules::DAC))
			CHECK_CALL(ES8388Interface::SetDACFormat(ES8388Interface::Formats::I2S));

		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
			CHECK_CALL(ES8388Interface::SetInputToMixerGain(6));

		if (Bitwise::IsEnabled(m_Modules, Modules::DAC))
			CHECK_CALL(ES8388Interface::SetDigitalVolume(0));

		CHECK_CALL(SetMicrophoneGain(24));

		CHECK_CALL(SetInputVolume(0));

		CHECK_CALL(SetOutputVolume(4.5F));

		CHECK_CALL(SetMute(false));
	}

	bool SetBitsPerSample(BitsPerSamples BitsPerSample)
	{
		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
			CHECK_CALL(ES8388Interface::SetADCBitsPerSample((ES8388Interface::BitsPerSamples)BitsPerSample));

		if (Bitwise::IsEnabled(m_Modules, Modules::DAC))
			CHECK_CALL(ES8388Interface::SetDACBitsPerSample((ES8388Interface::BitsPerSamples)BitsPerSample));

		return true;
	}

	BitsPerSamples GetBitsPerSample(void)
	{
		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
			return (BitsPerSamples)ES8388Interface::GetADCBitsPerSample();

		if (Bitwise::IsEnabled(m_Modules, Modules::DAC))
			return (BitsPerSamples)ES8388Interface::GetDACBitsPerSample();

		return BitsPerSamples::BPS16;
	}

	//[0dB, 24dB]
	bool SetMicrophoneGain(float dB)
	{
		if (!Bitwise::IsEnabled(m_Modules, Modules::ADC))
			return false;

		CHECK_CALL(ES8388Interface::SetMicrophoneGain(dB));

		return true;
	}

	float GetMicrophoneGain(void)
	{
		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
			return ES8388Interface::GetMicrophoneGain();

		return -1;
	}

	//[-96dB, 0dB]
	bool SetInputVolume(float dB)
	{
		if (!Bitwise::IsEnabled(m_Modules, Modules::ADC))
			return false;

		CHECK_CALL(ES8388Interface::SetInputVolume(dB));

		return true;
	}

	float GetInputVolume(void)
	{
		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
			return ES8388Interface::GetInputVolume();

		return -1;
	}

	//[-45dB, 4.5dB]
	bool SetOutputVolume(float dB)
	{
		if (!Bitwise::IsEnabled(m_Modules, Modules::DAC))
			return false;

		CHECK_CALL(ES8388Interface::SetOutputVolume(dB));

		return true;
	}

	float GetOutputVolume(void)
	{
		if (Bitwise::IsEnabled(m_Modules, Modules::DAC))
			return ES8388Interface::GetOutputVolume();

		return -1;
	}

	bool SetMute(bool Mute)
	{
		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
			CHECK_CALL(ES8388Interface::SetInputMute(Mute));

		if (Bitwise::IsEnabled(m_Modules, Modules::DAC))
			CHECK_CALL(ES8388Interface::SetOutputMute(Mute));

		return true;
	}

	bool GetMute(void)
	{
		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
			return ES8388Interface::GetInputMute();

		if (Bitwise::IsEnabled(m_Modules, Modules::DAC))
			return ES8388Interface::GetOutputMute();

		return false;
	}

	// Optimize the analog to digital conversion range
	//[0, 4]
	//(1Vrms/2.83Vpp, 0.5Vrms/1.41Vpp, 0.25Vrms/707mVpp, 0.125Vrms/354mVpp, 0.625Vrms/177mVpp)
	bool OptimizeConversion(uint Range = 2)
	{
		return ES8388Interface::OptimizeConversion(Range);
	}

private:
	Modules m_Modules;

	static const char *TAG;
};

const char *ES8388::TAG = "ES8388";

#endif