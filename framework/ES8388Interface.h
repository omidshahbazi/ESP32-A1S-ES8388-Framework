#pragma once
#ifndef ES8388_INTERFACE_H
#define ES8388_INTERFACE_H

#include "ES8388Control.h"
#include "Math.h"

class ES8388Interface
{
public:
	enum class MiddleVoltageResistances
	{
		Disabled = (uint8)ES8388Control::Values::ChipControl1_VMIDSEL_00,
		R5K = (uint8)ES8388Control::Values::ChipControl1_VMIDSEL_11,
		R50K = (uint8)ES8388Control::Values::ChipControl1_VMIDSEL_01,
		R500K = (uint8)ES8388Control::Values::ChipControl1_VMIDSEL_10
	};

	enum class OutputResistances
	{
		R1K5 = (uint8)ES8388Control::Values::DACControl23_VROI_0,
		R40K = (uint8)ES8388Control::Values::DACControl23_VROI_1,
	};

	enum class IOModes
	{
		Left1 = 0b00000001,
		Right1 = 0b00000010,
		Left2 = 0b00000100,
		Right2 = 0b00001000,
		Differentiate = 0b00010000,
		LeftAndRight1 = Left1 | Right1,
		LeftAndRight2 = Left2 | Right2,
		All = LeftAndRight1 | LeftAndRight2
	};

	enum class Formats
	{
		I2S = 0,
		LeftJustify,
		RightJustify,
		DSP_PCM
	};

	enum class BitsPerSamples
	{
		BPS16 = 16,
		BPS18 = 18,
		BPS20 = 20,
		BPS24 = 24,
		BPS32 = 32
	};

public:
	static bool TurnOn(bool MasterMode, MiddleVoltageResistances MiddleVoltageResistance)
	{
		Log::WriteInfo(TAG, "Turning On");

		CHECK_CALL(SetADCEnabled(true));
		CHECK_CALL(SetDACEnabled(true));

		CHECK_CALL(SetChipControls(MiddleVoltageResistance, false, true));

		ES8388Control::Write(ES8388Control::Registers::MasterMode, ES8388Control::Values::MasterMode_BCLKDIV_00000, ES8388Control::Masks::MasterMode_BCLKDIV);
		ES8388Control::Write(ES8388Control::Registers::MasterMode, ES8388Control::Values::MasterMode_BCLK_INV_0, ES8388Control::Masks::MasterMode_BCLK_INV);
		ES8388Control::Write(ES8388Control::Registers::MasterMode, ES8388Control::Values::MasterMode_MCLKDIV2_0, ES8388Control::Masks::MasterMode_MCLKDIV2);
		ES8388Control::Write(
			ES8388Control::Registers::MasterMode,
			(MasterMode ? ES8388Control::Values::MasterMode_MSC_1 : ES8388Control::Values::MasterMode_MSC_0),
			ES8388Control::Masks::MasterMode_MSC);

		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_dac_dll_pwd_0, ES8388Control::Masks::DACControl21_dac_dll_pwd);
		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_adc_dll_pwd_0, ES8388Control::Masks::DACControl21_adc_dll_pwd);
		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_mclk_dis_0, ES8388Control::Masks::DACControl21_mclk_dis);
		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_offset_dis_0, ES8388Control::Masks::DACControl21_offset_dis);
		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_lrck_sel_0, ES8388Control::Masks::DACControl21_lrck_sel);
		ES8388Control::Write(ES8388Control::Registers::DACControl21, ES8388Control::Values::DACControl21_slrck_1, ES8388Control::Masks::DACControl21_slrck);

		return true;
	}

	static bool TurnOff(void)
	{
		Log::WriteInfo(TAG, "Turning Off");

		SetADCEnabled(false);
		SetDACEnabled(false);

		CHECK_CALL(SetChipControls(MiddleVoltageResistances::Disabled, false, false));

		return true;
	}

	static bool Standby(void)
	{
		Log::WriteInfo(TAG, "Standing by");

		CHECK_CALL(SetInputMute(true));
		CHECK_CALL(SetOutputMute(true));

		CHECK_CALL(SetADCEnabled(false, false));
		CHECK_CALL(SetDACEnabled(false, false));

		CHECK_CALL(SetADCPowered(false, false, IOModes::All));
		CHECK_CALL(SetDACPowered(false, IOModes::All, OutputResistances::R1K5));

		return true;
	}

	static bool Resume(void)
	{
		Log::WriteInfo(TAG, "Resuming");

		CHECK_CALL(SetADCPowered(true, true, IOModes::All));
		CHECK_CALL(SetDACPowered(true, IOModes::All, OutputResistances::R1K5));

		CHECK_CALL(SetInputMute(false));
		CHECK_CALL(SetOutputMute(false));

		CHECK_CALL(SetADCEnabled(false));
		CHECK_CALL(SetDACEnabled(false));

		return true;
	}

	static bool SetADCPowered(bool Powered, bool MicrophoneBiasPowered, IOModes IOModes)
	{
		Log::WriteInfo(TAG, "Setting ADC Powered: %i, Microphone Bias Powered: %i, R: %i, L: %i", Powered, (Powered && MicrophoneBiasPowered),
					   Bitwise::IsEnabled(IOModes, IOModes::Right1 | IOModes::Right2),
					   Bitwise::IsEnabled(IOModes, IOModes::Left1 | IOModes::Left2));

		ES8388Control::Write(
			ES8388Control::Registers::ADCPower,
			(Powered ? ES8388Control::Values::ADCPower_int1LP_0 : ES8388Control::Values::ADCPower_int1LP_1), ES8388Control::Masks::ADCPower_int1LP);

		ES8388Control::Write(
			ES8388Control::Registers::ADCPower,
			(Powered ? ES8388Control::Values::ADCPower_flashLP_0 : ES8388Control::Values::ADCPower_flashLP_1), ES8388Control::Masks::ADCPower_flashLP);

		ES8388Control::Write(
			ES8388Control::Registers::ADCPower,
			(Powered ? ES8388Control::Values::ADCPower_PdnADCBiasgen_0 : ES8388Control::Values::ADCPower_PdnADCBiasgen_1), ES8388Control::Masks::ADCPower_PdnADCBiasgen);

		ES8388Control::Write(
			ES8388Control::Registers::ADCPower,
			(Powered && MicrophoneBiasPowered ? ES8388Control::Values::ADCPower_PdnMICB_0 : ES8388Control::Values::ADCPower_PdnMICB_1), ES8388Control::Masks::ADCPower_PdnMICB);

		if (Bitwise::IsEnabled(IOModes, IOModes::Right1 | IOModes::Right2))
		{
			ES8388Control::Write(
				ES8388Control::Registers::ADCPower,
				(Powered ? ES8388Control::Values::ADCPower_PdnADCR_0 : ES8388Control::Values::ADCPower_PdnADCR_1), ES8388Control::Masks::ADCPower_PdnADCR);

			ES8388Control::Write(
				ES8388Control::Registers::ADCPower,
				(Powered ? ES8388Control::Values::ADCPower_PdnAINR_0 : ES8388Control::Values::ADCPower_PdnAINR_1), ES8388Control::Masks::ADCPower_PdnAINR);
		}

		if (Bitwise::IsEnabled(IOModes, IOModes::Left1 | IOModes::Left2))
		{
			ES8388Control::Write(
				ES8388Control::Registers::ADCPower,
				(Powered ? ES8388Control::Values::ADCPower_PdnADCL_0 : ES8388Control::Values::ADCPower_PdnADCL_1), ES8388Control::Masks::ADCPower_PdnADCL);

			ES8388Control::Write(
				ES8388Control::Registers::ADCPower,
				(Powered ? ES8388Control::Values::ADCPower_PdnAINL_0 : ES8388Control::Values::ADCPower_PdnAINL_1), ES8388Control::Masks::ADCPower_PdnAINL);
		}

		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_DATSEL_00, ES8388Control::Masks::ADCControl4_DATSEL);

		return true;
	}

	static bool SetDACPowered(bool Powered, IOModes IOModes, OutputResistances OutputResistance)
	{
		Log::WriteInfo(TAG, "Setting DAC Powered: %i, R1: %i, L1: %i, R2: %i, L2: %i, Output Resistance: %i", Powered,
					   Bitwise::IsEnabled(IOModes, IOModes::Right1),
					   Bitwise::IsEnabled(IOModes, IOModes::Left1),
					   Bitwise::IsEnabled(IOModes, IOModes::Right2),
					   Bitwise::IsEnabled(IOModes, IOModes::Left2),
					   OutputResistance);

		if (Bitwise::IsEnabled(IOModes, IOModes::Right2))
			ES8388Control::Write(
				ES8388Control::Registers::DACPower,
				(Powered ? ES8388Control::Values::DACPower_ROUT2_1 : ES8388Control::Values::DACPower_ROUT2_0), ES8388Control::Masks::DACPower_ROUT2);

		if (Bitwise::IsEnabled(IOModes, IOModes::Left2))
			ES8388Control::Write(
				ES8388Control::Registers::DACPower,
				(Powered ? ES8388Control::Values::DACPower_LOUT2_1 : ES8388Control::Values::DACPower_LOUT2_0), ES8388Control::Masks::DACPower_LOUT2);

		if (Bitwise::IsEnabled(IOModes, IOModes::Right1))
			ES8388Control::Write(
				ES8388Control::Registers::DACPower,
				(Powered ? ES8388Control::Values::DACPower_ROUT1_1 : ES8388Control::Values::DACPower_ROUT1_0), ES8388Control::Masks::DACPower_ROUT1);

		if (Bitwise::IsEnabled(IOModes, IOModes::Left1))
			ES8388Control::Write(
				ES8388Control::Registers::DACPower,
				(Powered ? ES8388Control::Values::DACPower_LOUT1_1 : ES8388Control::Values::DACPower_LOUT1_0), ES8388Control::Masks::DACPower_LOUT1);

		if (Bitwise::IsEnabled(IOModes, IOModes::Right1 | IOModes::Right2))
			ES8388Control::Write(
				ES8388Control::Registers::DACPower,
				(Powered ? ES8388Control::Values::DACPower_PdnDACR_0 : ES8388Control::Values::DACPower_PdnDACR_1), ES8388Control::Masks::DACPower_PdnDACR);

		if (Bitwise::IsEnabled(IOModes, IOModes::Left1 | IOModes::Left2))
			ES8388Control::Write(
				ES8388Control::Registers::DACPower,
				(Powered ? ES8388Control::Values::DACPower_PdnDACL_0 : ES8388Control::Values::DACPower_PdnDACL_1), ES8388Control::Masks::DACPower_PdnDACL);

		ES8388Control::Write(ES8388Control::Registers::DACControl23, (ES8388Control::Values)OutputResistance, ES8388Control::Masks::DACControl23_VROI);

		return true;
	}

	static bool SetADCFormat(Formats Format)
	{
		ES8388Control::Values value;
		switch (Format)
		{
		case Formats::I2S:
			value = ES8388Control::Values::ADCControl4_ADCFORMAT_00;
			Log::WriteInfo(TAG, "Setting ADC Format: I2S");
			break;

		case Formats::LeftJustify:
			value = ES8388Control::Values::ADCControl4_ADCFORMAT_01;
			Log::WriteInfo(TAG, "Setting ADC Format: LeftJustify");
			break;

		case Formats::RightJustify:
			value = ES8388Control::Values::ADCControl4_ADCFORMAT_10;
			Log::WriteInfo(TAG, "Setting ADC Format: RightJustify");
			break;

		case Formats::DSP_PCM:
			value = ES8388Control::Values::ADCControl4_ADCFORMAT_11;
			Log::WriteInfo(TAG, "Setting ADC Format: DSP_PCM");
			break;

		default:
			return false;
		}

		ES8388Control::Write(ES8388Control::Registers::ADCControl4, value, ES8388Control::Masks::ADCControl4_ADCFORMAT);

		//  Left/Right data, Left/Right justified mode, Bits length, I2S format
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, ES8388Control::Values::ADCControl4_ADCLRP_0, ES8388Control::Masks::ADCControl4_ADCLRP);

		return true;
	}

	static bool SetDACFormat(Formats Format)
	{
		ES8388Control::Values value;
		switch (Format)
		{
		case Formats::I2S:
			value = ES8388Control::Values::DACControl1_DACFORMAT_00;
			Log::WriteInfo(TAG, "Setting DAC Format: I2S");
			break;

		case Formats::LeftJustify:
			value = ES8388Control::Values::DACControl1_DACFORMAT_01;
			Log::WriteInfo(TAG, "Setting DAC Format: LeftJustify");
			break;

		case Formats::RightJustify:
			value = ES8388Control::Values::DACControl1_DACFORMAT_10;
			Log::WriteInfo(TAG, "Setting DAC Format: RightJustify");
			break;

		case Formats::DSP_PCM:
			value = ES8388Control::Values::DACControl1_DACFORMAT_11;
			Log::WriteInfo(TAG, "Setting DAC Format: DSP_PCM");
			break;

		default:
			return false;
		}

		ES8388Control::Write(ES8388Control::Registers::DACControl1, value, ES8388Control::Masks::DACControl1_DACFORMAT);

		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACLRP_0, ES8388Control::Masks::DACControl1_DACLRP);

		return true;
	}

	static bool SetADCBitsPerSample(BitsPerSamples BPS)
	{
		ES8388Control::Values value;
		switch (BPS)
		{
		case BitsPerSamples::BPS16:
			value = ES8388Control::Values::ADCControl4_ADCWL_011;
			break;

		case BitsPerSamples::BPS18:
			value = ES8388Control::Values::ADCControl4_ADCWL_010;
			break;

		case BitsPerSamples::BPS20:
			value = ES8388Control::Values::ADCControl4_ADCWL_001;
			break;

		case BitsPerSamples::BPS24:
			value = ES8388Control::Values::ADCControl4_ADCWL_000;
			break;

		case BitsPerSamples::BPS32:
			value = ES8388Control::Values::ADCControl4_ADCWL_100;
			break;

		default:
			return false;
		}

		Log::WriteInfo(TAG, "Setting I2S ADC Bits Per Sample: %i", BPS);

		ES8388Control::Write(ES8388Control::Registers::ADCControl4, value, ES8388Control::Masks::ADCControl4_ADCWL);

		return true;
	}

	static BitsPerSamples GetADCBitsPerSample(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::ADCControl4, ES8388Control::Masks::ADCControl4_ADCWL);

		switch (value)
		{
		case ES8388Control::Values::ADCControl4_ADCWL_011:
			return BitsPerSamples::BPS16;

		case ES8388Control::Values::ADCControl4_ADCWL_010:
			return BitsPerSamples::BPS18;

		case ES8388Control::Values::ADCControl4_ADCWL_001:
			return BitsPerSamples::BPS20;

		case ES8388Control::Values::ADCControl4_ADCWL_000:
			return BitsPerSamples::BPS24;

		case ES8388Control::Values::ADCControl4_ADCWL_100:
			return BitsPerSamples::BPS32;

		default:
			return (BitsPerSamples)0;
		}
	}

	static bool SetDACBitsPerSample(BitsPerSamples BPS)
	{
		ES8388Control::Values value;
		switch (BPS)
		{
		case BitsPerSamples::BPS16:
			value = ES8388Control::Values::DACControl1_DACWL_011;
			break;

		case BitsPerSamples::BPS18:
			value = ES8388Control::Values::DACControl1_DACWL_010;
			break;

		case BitsPerSamples::BPS20:
			value = ES8388Control::Values::DACControl1_DACWL_001;
			break;

		case BitsPerSamples::BPS24:
			value = ES8388Control::Values::DACControl1_DACWL_000;
			break;

		case BitsPerSamples::BPS32:
			value = ES8388Control::Values::DACControl1_DACWL_100;
			break;

		default:
			return false;
		}

		Log::WriteInfo(TAG, "Setting I2S DAC Bits Per Sample: %i", BPS);

		ES8388Control::Write(ES8388Control::Registers::DACControl1, value, ES8388Control::Masks::DACControl1_DACWL);

		return true;
	}

	static BitsPerSamples GetDACBitsPerSample(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::ADCControl1, ES8388Control::Masks::DACControl1_DACWL);

		switch (value)
		{
		case ES8388Control::Values::DACControl1_DACWL_011:
			return BitsPerSamples::BPS16;

		case ES8388Control::Values::DACControl1_DACWL_010:
			return BitsPerSamples::BPS18;

		case ES8388Control::Values::DACControl1_DACWL_001:
			return BitsPerSamples::BPS20;

		case ES8388Control::Values::DACControl1_DACWL_000:
			return BitsPerSamples::BPS24;

		case ES8388Control::Values::DACControl1_DACWL_100:
			return BitsPerSamples::BPS32;
		}

		return (BitsPerSamples)0;
	}

	//[0dB, 24dB]
	static bool SetMicrophoneGain(float dB)
	{
		dB = Math::Clamp(dB, 0, 24);

		Log::WriteInfo(TAG, "Setting Microphone Gain: %.1fdB", dB);

		uint8 value = (uint8)(dB / 3);

		ES8388Control::Write(ES8388Control::Registers::ADCControl1, (ES8388Control::Values)(value & (uint8)ES8388Control::Masks::ADCControl1_MicAmpR), ES8388Control::Masks::ADCControl1_MicAmpR);
		ES8388Control::Write(ES8388Control::Registers::ADCControl1, (ES8388Control::Values)((value << 4) & (uint8)ES8388Control::Masks::ADCControl1_MicAmpL), ES8388Control::Masks::ADCControl1_MicAmpL);

		return true;
	}

	static float GetMicrophoneGain(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::ADCControl1, ES8388Control::Masks::ADCControl1_MicAmpR);

		return (uint8)value * 3.0F;
	}

	//[-76.5dBFS, -30dBFS]
	static bool SetMicrophoneNoiseGate(float dbFS)
	{
		dbFS = Math::Clamp(dbFS, -76.5, -30);

		Log::WriteInfo(TAG, "Setting Microphone Noise gate: %.1fdBFS", dbFS);

		uint8 value = (dbFS + 76.5) / -1.5;

		ES8388Control::Write(ES8388Control::Registers::ADCControl14, (ES8388Control::Values)(value << 3), ES8388Control::Masks::ADCControl14_NGTH);

		return true;
	}

	static float GetMicrophoneNoiseGate(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::DACControl17, ES8388Control::Masks::DACControl17_LI2LOVOL);

		return (7 - ((uint8)value >> 3) * 3.0F) - 15;
	}

	//[-15dB, 6dB]
	static bool SetInputToMixerGain(float dB)
	{
		dB = Math::Clamp(dB, -15, 6);

		Log::WriteInfo(TAG, "Setting Input to Mixer gain: %.1fdB", dB);

		uint8 value = 7 - (uint8)((dB + 15) / 3.0F);

		ES8388Control::Write(ES8388Control::Registers::DACControl17, (ES8388Control::Values)(value << 3), ES8388Control::Masks::DACControl17_LI2LOVOL);
		ES8388Control::Write(ES8388Control::Registers::DACControl20, (ES8388Control::Values)(value << 3), ES8388Control::Masks::DACControl20_RI2ROVOL);

		return true;
	}

	static float GetInputToMixerGain(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::DACControl17, ES8388Control::Masks::DACControl17_LI2LOVOL);

		return ((7 - ((uint8)value >> 3)) * 3.0F) - 15;
	}

	//[-96dB, 0dB]
	static bool SetDigitalVolume(float dB)
	{
		dB = Math::Clamp(dB, -96, 0);

		Log::WriteInfo(TAG, "Setting DAC volume: %.1fdB", dB);

		ES8388Control::Values value = (ES8388Control::Values)(((uint8)(dB * 2)) & (uint8)ES8388Control::Masks::DACControl4_LDACVOL);

		ES8388Control::Write(ES8388Control::Registers::DACControl4, value, ES8388Control::Masks::DACControl4_LDACVOL);
		ES8388Control::Write(ES8388Control::Registers::DACControl5, value, ES8388Control::Masks::DACControl5_RDACVOL);

		return true;
	}

	static float GetDigitalVolume(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::DACControl4, ES8388Control::Masks::DACControl4_LDACVOL);

		return (uint8)value / 2.0F;
	}

	//[-96dB, 0dB]
	static bool SetInputVolume(float dB)
	{
		dB = Math::Clamp(dB, -96, 0);

		Log::WriteInfo(TAG, "Setting Input volume: %.1fdB", dB);

		ES8388Control::Values value = (ES8388Control::Values)(((uint8)(dB * 2)) & (uint8)ES8388Control::Masks::ADCControl8_LADCVOL);

		ES8388Control::Write(ES8388Control::Registers::ADCControl8, value, ES8388Control::Masks::ADCControl8_LADCVOL);
		ES8388Control::Write(ES8388Control::Registers::ADCControl9, value, ES8388Control::Masks::ADCControl9_RADCVOL);

		return true;
	}

	static float GetInputVolume(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::ADCControl8, ES8388Control::Masks::ADCControl8_LADCVOL);

		return (uint8)value / 2.0F;
	}

	//[-45dB, 4.5dB]
	static bool SetOutputVolume(float dB)
	{
		dB = Math::Clamp(dB, -45, 4.5F);

		Log::WriteInfo(TAG, "Setting Output Volume: %.1fdB", dB);

		ES8388Control::Values value = (ES8388Control::Values)((uint8)((dB + 45) / 1.5F) & (uint8)ES8388Control::Masks::DACControl24_LOUT1VOL);

		ES8388Control::Write(ES8388Control::Registers::DACControl24, (ES8388Control::Values)value, ES8388Control::Masks::DACControl24_LOUT1VOL);
		ES8388Control::Write(ES8388Control::Registers::DACControl25, (ES8388Control::Values)value, ES8388Control::Masks::DACControl25_ROUT1VOL);
		ES8388Control::Write(ES8388Control::Registers::DACControl26, (ES8388Control::Values)value, ES8388Control::Masks::DACControl26_LOUT2VOL);
		ES8388Control::Write(ES8388Control::Registers::DACControl27, (ES8388Control::Values)value, ES8388Control::Masks::DACControl27_ROUT2VOL);

		return true;
	}

	static float GetOutputVolume(void)
	{
		ES8388Control::Values value = ES8388Control::Read(ES8388Control::Registers::DACControl24, ES8388Control::Masks::DACControl24_LOUT1VOL);

		return ((uint8)value * 1.5F) - 45;
	}

	static bool SetInputMute(bool Mute)
	{
		Log::WriteInfo(TAG, "Setting input Mute: %i", Mute);

		ES8388Control::Write(
			ES8388Control::Registers::ADCControl7,
			(Mute ? ES8388Control::Values::ADCControl7_ADCMute_1 : ES8388Control::Values::ADCControl7_ADCMute_0),
			ES8388Control::Masks::ADCControl7_ADCMute);

		return true;
	}

	static bool GetInputMute(void)
	{
		return (ES8388Control::Read(ES8388Control::Registers::ADCControl7, ES8388Control::Masks::ADCControl7_ADCMute) == ES8388Control::Values::ADCControl7_ADCMute_1);
	}

	static bool SetOutputMute(bool Mute)
	{
		Log::WriteInfo(TAG, "Setting output Mute: %i", Mute);

		ES8388Control::Write(
			ES8388Control::Registers::DACControl3,
			(Mute ? ES8388Control::Values::DACControl3_DACMute_1 : ES8388Control::Values::DACControl3_DACMute_0),
			ES8388Control::Masks::DACControl3_DACMute);

		return true;
	}

	static bool GetOutputMute(void)
	{
		return (ES8388Control::Read(ES8388Control::Registers::DACControl3, ES8388Control::Masks::DACControl3_DACMute) == ES8388Control::Values::DACControl3_DACMute_1);
	}

	// Optimize the analog to digital conversion range
	//[0, 4]
	//(1Vrms/2.83Vpp, 0.5Vrms/1.41Vpp, 0.25Vrms/707mVpp, 0.125Vrms/354mVpp, 0.625Vrms/177mVpp)
	static bool OptimizeConversion(uint8 Range = 2)
	{
		static float INPUT_GAIN[] = {0, 6, 12, 18, 24};
		static float OUTPUT_VOLUME[] = {0, -6, -12, -18, -24};

		Range = Math::Clamp(Range, 0, 4);

		Log::WriteInfo(TAG, "Optimizing Conversion: %i, InputGain: %idb, Output Volume: %idb", Range, INPUT_GAIN[Range], OUTPUT_VOLUME[Range]);

		CHECK_CALL(SetMicrophoneGain(INPUT_GAIN[Range]));
		CHECK_CALL(SetOutputVolume(OUTPUT_VOLUME[Range]));

		return true;
	}

private:
	static bool SetChipControls(MiddleVoltageResistances MiddleVoltageResistance, bool ResetControlPort, bool PoweredOn)
	{
		Log::WriteInfo(TAG, "Setting Chip Control Middle Voltage Resistance: %i, Reset Control Port: %i, Powered On: %i", MiddleVoltageResistance, ResetControlPort, PoweredOn);

		ES8388Control::Write(ES8388Control::Registers::ChipControl1, (ES8388Control::Values)MiddleVoltageResistance, ES8388Control::Masks::ChipControl1_VMIDSEL);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_EnRef_0, ES8388Control::Masks::ChipControl1_EnRef);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_SeqEn_0, ES8388Control::Masks::ChipControl1_SeqEn);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_SameFs_1, ES8388Control::Masks::ChipControl1_SameFs);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_DACMCLK_0, ES8388Control::Masks::ChipControl1_DACMCLK);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_LRCM_0, ES8388Control::Masks::ChipControl1_LRCM);
		ES8388Control::Write(
			ES8388Control::Registers::ChipControl1,
			(ResetControlPort ? ES8388Control::Values::ChipControl1_SCPReset_1 : ES8388Control::Values::ChipControl1_SCPReset_0),
			ES8388Control::Masks::ChipControl1_SCPReset);

		if (PoweredOn)
		{
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_PdnVrefbuf_0, ES8388Control::Masks::ChipControl2_PdnVrefbuf);
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_VrefLo_0, ES8388Control::Masks::ChipControl2_VrefLo);
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_PdnIbiasgen_0, ES8388Control::Masks::ChipControl2_PdnIbiasgen);
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_PdnAna_0, ES8388Control::Masks::ChipControl2_PdnAna);
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_LPVrefBuf_1, ES8388Control::Masks::ChipControl2_LPVrefBuf);
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_LPVcmMod_0, ES8388Control::Masks::ChipControl2_LPVcmMod);
		}
		else
		{
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_PdnVrefbuf_1, ES8388Control::Masks::ChipControl2_PdnVrefbuf);
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_VrefLo_1, ES8388Control::Masks::ChipControl2_VrefLo);
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_PdnIbiasgen_1, ES8388Control::Masks::ChipControl2_PdnIbiasgen);
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_PdnAna_1, ES8388Control::Masks::ChipControl2_PdnAna);
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_LPVrefBuf_1, ES8388Control::Masks::ChipControl2_LPVrefBuf);
			ES8388Control::Write(ES8388Control::Registers::ChipControl2, ES8388Control::Values::ChipControl2_LPVcmMod_1, ES8388Control::Masks::ChipControl2_LPVcmMod);
		}

		return true;
	}

	static bool SetADCEnabled(bool Enabled, bool ChangeDLL = true)
	{
		Log::WriteInfo(TAG, "Setting ADC Enabled: %i, ChangeDLL: %i", Enabled, ChangeDLL);

		if (Enabled)
		{
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adcVref_PDN_0, ES8388Control::Masks::ChipPower_adcVref_PDN);

			if (ChangeDLL)
				ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_ADCDLL_PDN_0, ES8388Control::Masks::ChipPower_ADCDLL_PDN);

			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adc_stm_rst_0, ES8388Control::Masks::ChipPower_adc_stm_rst);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adc_DigPDN_0, ES8388Control::Masks::ChipPower_adc_DigPDN);
		}
		else
		{
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adcVref_PDN_1, ES8388Control::Masks::ChipPower_adcVref_PDN);

			if (ChangeDLL)
				ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_ADCDLL_PDN_1, ES8388Control::Masks::ChipPower_ADCDLL_PDN);

			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adc_stm_rst_1, ES8388Control::Masks::ChipPower_adc_stm_rst);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_adc_DigPDN_1, ES8388Control::Masks::ChipPower_adc_DigPDN);
		}

		ES8388Control::Write(ES8388Control::Registers::ADCControl5, ES8388Control::Values::ADCControl5_ADCFsRatio_00010, ES8388Control::Masks::ADCControl5_ADCFsRatio);
		ES8388Control::Write(ES8388Control::Registers::ADCControl5, ES8388Control::Values::ADCControl5_ADCFsMode_0, ES8388Control::Masks::ADCControl5_ADCFsMode);

		return true;
	}

	static bool SetDACEnabled(bool Enabled, bool ChangeDLL = true)
	{
		Log::WriteInfo(TAG, "Setting DAC Enabled: %i, ChangeDLL: %i", Enabled, ChangeDLL);

		if (Enabled)
		{
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dacVref_PDN_0, ES8388Control::Masks::ChipPower_dacVref_PDN);

			if (ChangeDLL)
				ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_DACDLL_PDN_0, ES8388Control::Masks::ChipPower_DACDLL_PDN);

			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dac_stm_rst_0, ES8388Control::Masks::ChipPower_dac_stm_rst);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dac_DigPDN_0, ES8388Control::Masks::ChipPower_dac_DigPDN);
		}
		else
		{
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dacVref_PDN_1, ES8388Control::Masks::ChipPower_dacVref_PDN);

			if (ChangeDLL)
				ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_DACDLL_PDN_1, ES8388Control::Masks::ChipPower_DACDLL_PDN);

			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dac_stm_rst_1, ES8388Control::Masks::ChipPower_dac_stm_rst);
			ES8388Control::Write(ES8388Control::Registers::ChipPower, ES8388Control::Values::ChipPower_dac_DigPDN_1, ES8388Control::Masks::ChipPower_dac_DigPDN);
		}

		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACLRSWAP_0, ES8388Control::Masks::DACControl1_DACLRSWAP);

		ES8388Control::Write(ES8388Control::Registers::DACControl2, ES8388Control::Values::DACCOntrol2_DACFsRatio_00010, ES8388Control::Masks::DACCOntrol2_DACFsRatio);
		ES8388Control::Write(ES8388Control::Registers::DACControl2, ES8388Control::Values::DACCOntrol2_DACFsMode_0, ES8388Control::Masks::DACCOntrol2_DACFsMode);

		ES8388Control::Write(ES8388Control::Registers::DACControl16, ES8388Control::Values::DACControl16_RMIXSEL_000, ES8388Control::Masks::DACControl16_RMIXSEL);
		ES8388Control::Write(ES8388Control::Registers::DACControl16, ES8388Control::Values::DACControl16_LMIXSEL_000, ES8388Control::Masks::DACControl16_LMIXSEL);

		ES8388Control::Write(ES8388Control::Registers::DACControl17, ES8388Control::Values::DACControl17_LI2LO_0, ES8388Control::Masks::DACControl17_LI2LO);
		ES8388Control::Write(ES8388Control::Registers::DACControl17, ES8388Control::Values::DACControl17_LD2LO_1, ES8388Control::Masks::DACControl17_LD2LO);

		ES8388Control::Write(ES8388Control::Registers::DACControl20, ES8388Control::Values::DACControl20_RI2RO_0, ES8388Control::Masks::DACControl20_RI2RO);
		ES8388Control::Write(ES8388Control::Registers::DACControl20, ES8388Control::Values::DACControl20_RD2RO_1, ES8388Control::Masks::DACControl20_RD2RO);

		return true;
	}

private:
	static const char *TAG;
};

const char *ES8388Interface::TAG = "ES8388Interface";

#endif