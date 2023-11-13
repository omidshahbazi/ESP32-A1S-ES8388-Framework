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
	enum class BitsPerSamples
	{
		BPS16 = (uint8)ES8388Interface::BitsPerSamples::BPS16,
		BPS24 = (uint8)ES8388Interface::BitsPerSamples::BPS24,
		BPS32 = (uint8)ES8388Interface::BitsPerSamples::BPS32
	};

	// TODO: Renaming
	enum class InputModes
	{
		None = (uint8)ES8388Interface::InputModes::None,

		Left1 = (uint8)ES8388Interface::InputModes::Left1,
		Right1 = (uint8)ES8388Interface::InputModes::Right1,

		Left2 = (uint8)ES8388Interface::InputModes::Left2,
		Right2 = (uint8)ES8388Interface::InputModes::Right2,
	};

	enum class OutputModes
	{
		None = (uint8)ES8388Interface::OutputModes::None,
		Left1 = (uint8)ES8388Interface::OutputModes::Left1,
		Right1 = (uint8)ES8388Interface::OutputModes::Right1,
		Left2 = (uint8)ES8388Interface::OutputModes::Left2,
		Right2 = (uint8)ES8388Interface::OutputModes::Right2,

		Left1AndLeft2 = Left1 | Left2,
		Right1AndRight2 = Right1 | Right2,

		LeftAndRight1 = Left1 | Right1,
		LeftAndRight2 = Left2 | Right2,

		All = (uint8)ES8388Interface::OutputModes::All
	};

public:
	ES8388(InputModes InputMode, OutputModes OutputMode)
		: m_InputMode(InputMode),
		  m_OutputMode(OutputMode)
	{
		Log::WriteInfo(TAG, "Intializing");

		CHECK_CALL(ES8388Interface::TurnOn(false, ES8388Interface::MiddleVoltageResistances::R50K));

		bool needsADC = (m_InputMode != InputModes::None);
		bool needsDAC = (m_OutputMode != OutputModes::None);

		if (needsADC)
			CHECK_CALL(ES8388Interface::SetADCPowered(true, (ES8388Interface::InputModes)InputMode));

		if (needsADC)
			CHECK_CALL(ES8388Interface::SetDACPowered(true, (ES8388Interface::OutputModes)OutputMode, ES8388Interface::OutputResistances::R1K5));

		CHECK_CALL(SetBitsPerSample(BitsPerSamples::BPS16));

		if (needsADC)
			CHECK_CALL(ES8388Interface::SetADCFormat(ES8388Interface::Formats::I2S));

		if (needsDAC)
			CHECK_CALL(ES8388Interface::SetDACFormat(ES8388Interface::Formats::I2S));

		if (needsADC)
		{
			CHECK_CALL(ES8388Interface::SetInputToMixerGain(6));

			// CHECK_CALL(ES8388Interface::SetMicrophoneNoiseGateEnabled(true));
			// CHECK_CALL(ES8388Interface::SetAutomaticLevelControlEnabled(true, (ES8388Interface::InputModes)InputMode));

			// if (Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1))
			// {
			// 	CHECK_CALL(ES8388Interface::SetMicrophoneNoiseGateParameters(-40.5F, true));
			// 	CHECK_CALL(SetAutomaticLevelControlParameters(0, 23.5F, -4.5F, 0, 0.416F, 0.820F, 21, false, false, false)); // Optimized for Microphone
			// }
			// else if (Bitwise::IsEnabled(InputMode, InputModes::Left2) || Bitwise::IsEnabled(InputMode, InputModes::Right2))
			// {
			// 	CHECK_CALL(ES8388Interface::SetMicrophoneNoiseGateParameters(-60, false));
			// 	CHECK_CALL(SetAutomaticLevelControlParameters(-12, 35.5F, -12, 0, 6.66F, 420, 21, false, false, false)); // Optimized for Music
			// }

			CHECK_CALL(SetMicrophoneGain(24));

			CHECK_CALL(SetInputVolume(0));
		}

		if (needsDAC)
		{
			CHECK_CALL(ES8388Interface::SetDigitalVolume(0));
			CHECK_CALL(SetOutputVolume(4.5F));
		}

		CHECK_CALL(SetMute(false));
	}

	bool SetBitsPerSample(BitsPerSamples BitsPerSample)
	{
		if (m_InputMode != InputModes::None)
			CHECK_CALL(ES8388Interface::SetADCBitsPerSample((ES8388Interface::BitsPerSamples)BitsPerSample));

		if (m_OutputMode != OutputModes::None)
			CHECK_CALL(ES8388Interface::SetDACBitsPerSample((ES8388Interface::BitsPerSamples)BitsPerSample));

		return true;
	}

	BitsPerSamples GetBitsPerSample(void)
	{
		if (m_InputMode != InputModes::None)
			return (BitsPerSamples)ES8388Interface::GetADCBitsPerSample();

		if (m_OutputMode != OutputModes::None)
			return (BitsPerSamples)ES8388Interface::GetDACBitsPerSample();

		return BitsPerSamples::BPS16;
	}

	// dBMin [-12dB, 30dB]
	// dBMax [-6.5dB, 35.5dB]
	// dBTarget [-16.5dB, -1.5dB]
	// HoldTime [0ms, 1360ms]
	// AttackTime [0.104ms/0.0227ms, 106ms/23.2ms]
	// DecayTime [0.410ms/0.0908ms, 420ms/93ms]
	bool SetAutomaticLevelControlParameters(float dBMin, float dBMax, float dBTarget, float HoldTime, float AttackTime, float DecayTime, uint8 WindowsSize, bool ZeroCrossTimeout, bool UseZeroCrossDetection, bool LimiterMode)
	{
		if (m_InputMode == InputModes::None)
			return false;

		CHECK_CALL(ES8388Interface::SetAutomaticLevelControlParameters(dBMin, dBMax, dBTarget, HoldTime, AttackTime, DecayTime, WindowsSize, ZeroCrossTimeout, UseZeroCrossDetection, LimiterMode));

		return true;
	}

	//[0dB, 24dB]
	bool SetMicrophoneGain(float dB)
	{
		if (m_InputMode == InputModes::None)
			return false;

		CHECK_CALL(ES8388Interface::SetMicrophoneGain((ES8388Interface::InputModes)m_InputMode, dB));

		return true;
	}

	float GetMicrophoneGain(void)
	{
		if (m_InputMode == InputModes::None)
			return -1;

		return ES8388Interface::GetMicrophoneGain();
	}

	//[-96dB, 0dB]
	bool SetInputVolume(float dB)
	{
		if (m_InputMode == InputModes::None)
			return false;

		CHECK_CALL(ES8388Interface::SetInputVolume(dB));

		return true;
	}

	float GetInputVolume(void)
	{
		if (m_InputMode == InputModes::None)
			return -1;

		return ES8388Interface::GetInputVolume();
	}

	//[-45dB, 4.5dB]
	bool SetOutputVolume(float dB)
	{
		if (m_InputMode == InputModes::None)
			return false;

		CHECK_CALL(ES8388Interface::SetOutputVolume(dB));

		return true;
	}

	float GetOutputVolume(void)
	{
		if (m_OutputMode == OutputModes::None)
			return -1;

		return ES8388Interface::GetOutputVolume();
	}

	bool SetMute(bool Mute)
	{
		if (m_InputMode != InputModes::None)
			CHECK_CALL(ES8388Interface::SetInputMute(Mute));

		if (m_OutputMode != OutputModes::None)
			CHECK_CALL(ES8388Interface::SetOutputMute(Mute));

		return true;
	}

	bool GetMute(void)
	{
		if (m_InputMode != InputModes::None)
			return ES8388Interface::GetInputMute();

		if (m_OutputMode != OutputModes::None)
			return ES8388Interface::GetOutputMute();

		return false;
	}

	// Optimize the analog to digital conversion range
	//[0, 4]
	//(1Vrms/2.83Vpp, 0.5Vrms/1.41Vpp, 0.25Vrms/707mVpp, 0.125Vrms/354mVpp, 0.625Vrms/177mVpp)
	bool OptimizeConversion(uint8 Range = 2)
	{
		if (m_InputMode == InputModes::None)
			return false;

		if (m_OutputMode == OutputModes::None)
			return false;

		static float INPUT_GAIN[] = {0, 6, 12, 18, 24};
		static float OUTPUT_VOLUME[] = {0, -6, -12, -18, -24};

		Range = Math::Clamp(Range, 0, 4);

		Log::WriteInfo(TAG, "Optimizing Conversion: %i, Input Gain: %.1ffdB, Output Volume: %.1ffdB", Range, INPUT_GAIN[Range], OUTPUT_VOLUME[Range]);

		CHECK_CALL(SetMicrophoneGain(INPUT_GAIN[Range]));
		CHECK_CALL(SetOutputVolume(OUTPUT_VOLUME[Range]));

		return true;
	}

private:
	InputModes m_InputMode;
	OutputModes m_OutputMode;

	static const char *TAG;
};

const char *ES8388::TAG = "ES8388";

#endif