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
private:
	enum class Modules
	{
		ADC = 0x1,
		DAC = 0x2,
		Both = ADC | DAC
	};

public:
	enum class BitsPerSamples
	{
		BPS16 = (uint8)ES8388Interface::BitsPerSamples::BPS16,
		BPS24 = (uint8)ES8388Interface::BitsPerSamples::BPS24,
		BPS32 = (uint8)ES8388Interface::BitsPerSamples::BPS32
	};

	enum class InputModes
	{
		None = 0b00000000,
		LeftAndRightInput1 = 0b00000001,
		Microphone1 = 0b00000010,
		Microphone2 = 0b00000100,
		LeftAndRightInput2 = 0b00001000,
		Difference = 0b00010000
	};

	enum class OutputModes
	{
		None = (uint8)ES8388Interface::OutputModes::None,
		Left1 = (uint8)ES8388Interface::OutputModes::Left1,
		Right1 = (uint8)ES8388Interface::OutputModes::Right1,
		Left2 = (uint8)ES8388Interface::OutputModes::Left2,
		Right2 = (uint8)ES8388Interface::OutputModes::Right2,
		All = (uint8)ES8388Interface::OutputModes::All
	};

public:
	ES8388(InputModes InputMode, OutputModes OutputMode)
		: m_Modules((Modules)0)
	{
		Log::WriteInfo(TAG, "Intializing");

		if (InputMode != InputModes::None)
			m_Modules |= Modules::ADC;
		if (OutputMode != OutputModes::None)
			m_Modules |= Modules::DAC;

		CHECK_CALL(ES8388Interface::TurnOn(false, ES8388Interface::MiddleVoltageResistances::R50K));

		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
		{
			CHECK_CALL(ES8388Interface::SetADCPowered(true, false, (ES8388Interface::InputModes)InputMode));

			CHECK_CALL(ES8388Interface::SetAutomaticLevelControlEnabled(true, (ES8388Interface::InputModes)InputMode));
		}

		if (Bitwise::IsEnabled(m_Modules, Modules::DAC))
			CHECK_CALL(ES8388Interface::SetDACPowered(true, (ES8388Interface::OutputModes)OutputMode, ES8388Interface::OutputResistances::R1K5));

		CHECK_CALL(SetBitsPerSample(BitsPerSamples::BPS16));

		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
			CHECK_CALL(ES8388Interface::SetADCFormat(ES8388Interface::Formats::I2S));

		if (Bitwise::IsEnabled(m_Modules, Modules::DAC))
			CHECK_CALL(ES8388Interface::SetDACFormat(ES8388Interface::Formats::I2S));

		if (Bitwise::IsEnabled(m_Modules, Modules::ADC))
		{
			CHECK_CALL(ES8388Interface::SetMicrophoneNoiseGate(-40.5F));
			CHECK_CALL(ES8388Interface::SetInputToMixerGain(6));

			CHECK_CALL(SetAutomaticLevelControlParameters(0, 0, -1.5F, 500, 50, 200));

			CHECK_CALL(SetMicrophoneGain(24));

			CHECK_CALL(SetInputVolume(0));
		}

		if (Bitwise::IsEnabled(m_Modules, Modules::DAC))
		{
			CHECK_CALL(ES8388Interface::SetDigitalVolume(0));
			CHECK_CALL(SetOutputVolume(4.5F));
		}

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

	// dBMin [-12dB, 30dB]
	// dBMax [-6.5dB, 35.5dB]
	// dBTarget [-16.5dB, -1.5dB]
	// HoldTime [0ms, 1360ms]
	// AttackTime [0.104ms/0.0227ms, 106ms/23.2ms]
	// DecayTime [0.410ms/0.0908ms, 420ms/93ms]
	bool SetAutomaticLevelControlParameters(float dBMin, float dBMax, float dBTarget, float HoldTime, float AttackTime, float DecayTime)
	{
		if (!Bitwise::IsEnabled(m_Modules, Modules::ADC))
			return false;

		CHECK_CALL(ES8388Interface::SetAutomaticLevelControlParameters(dBMin, dBMax, dBTarget, HoldTime, AttackTime, DecayTime));

		return true;
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
	bool OptimizeConversion(uint8 Range = 2)
	{
		return ES8388Interface::OptimizeConversion(Range);
	}

private:
	Modules m_Modules;

	static const char *TAG;
};

const char *ES8388::TAG = "ES8388";

#endif