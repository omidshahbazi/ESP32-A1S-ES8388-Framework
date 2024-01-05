#pragma once
#ifndef ES8388_INTERFACE_H
#define ES8388_INTERFACE_H

#include "ES8388Control.h"
#include "../Math.h"

// https://dl.radxa.com/rock2/docs/hw/ds/ES8388%20user%20Guide.pdf
class ES8388Interface
{
private:
	enum class InputModeBits
	{
		Left1 = 0b00000001,
		Right1 = 0b00000010,

		Left2 = 0b00000100,
		Right2 = 0b00001000,

		Differential = 0b00010000
	};

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

	enum class MonoMixModes
	{
		None = 0b00000000,
		MonoMixToLeft = 0b00000001,
		MonoMixToRight = 0b00000010
	};

	enum class InputModes
	{
		None = 0b00000000,

		Left1 = (uint8)InputModeBits::Left1,
		Right1 = (uint8)InputModeBits::Right1,
		Left2 = (uint8)InputModeBits::Left2,
		Right2 = (uint8)InputModeBits::Right2,

		Left1AndRight1 = Left1 | Right1,
		Left2AndRight2 = Left2 | Right2,

		Left1AndRight1Differential = (uint8)InputModeBits::Differential | Left1AndRight1,
		Left2AndRight2Differential = (uint8)InputModeBits::Differential | Left2AndRight2,
	};

	enum class OutputModes
	{
		None = 0b00000000,

		Left1 = 0b00000001,
		Right1 = 0b00000010,

		Left2 = 0b00000100,
		Right2 = 0b00001000
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
		Log::WriteDebug(TAG, "Turning On");

		CHECK_CALL(SetADCEnabled(true));
		CHECK_CALL(SetDACEnabled(true));

		CHECK_CALL(SetChipControls(MiddleVoltageResistance, false, true));

		ES8388Control::Write(ES8388Control::Registers::MasterMode, ES8388Control::Values::MasterMode_BCLKDIV_00000, ES8388Control::Masks::MasterMode_BCLKDIV);
		ES8388Control::Write(ES8388Control::Registers::MasterMode, ES8388Control::Values::MasterMode_BCLK_INV_0, ES8388Control::Masks::MasterMode_BCLK_INV);
		ES8388Control::Write(ES8388Control::Registers::MasterMode, ES8388Control::Values::MasterMode_MCLKDIV2_0, ES8388Control::Masks::MasterMode_MCLKDIV2);
		ES8388Control::Write(ES8388Control::Registers::MasterMode, (MasterMode ? ES8388Control::Values::MasterMode_MSC_1 : ES8388Control::Values::MasterMode_MSC_0), ES8388Control::Masks::MasterMode_MSC);

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
		Log::WriteDebug(TAG, "Turning Off");

		SetADCEnabled(false);
		SetDACEnabled(false);

		CHECK_CALL(SetChipControls(MiddleVoltageResistances::Disabled, false, false));

		return true;
	}

	// This needs to act based on the initial Configs, i.e. it shouldn't turn on/off all of the Outputs regarless of the initial Configs
	//  static bool Standby(void)
	//  {
	//  	Log::WriteDebug(TAG, "Standing by");

	// 	CHECK_CALL(SetInputMute(true));
	// 	CHECK_CALL(SetOutputMute(true));

	// 	CHECK_CALL(SetADCEnabled(false, false));
	// 	CHECK_CALL(SetDACEnabled(false, false));

	// 	CHECK_CALL(SetADCPowered(false, InputModes::BothDifferential));
	// 	CHECK_CALL(SetDACPowered(false, OutputModes::All, OutputResistances::R1K5));

	// 	return true;
	// }

	// This needs to act based on the initial Configs, i.e. it shouldn't turn on/off all of the Outputs regarless of the initial Configs
	//  static bool Resume(void)
	//  {
	//  	Log::WriteDebug(TAG, "Resuming");

	// 	CHECK_CALL(SetADCPowered(true, InputModes::BothDifferential));
	// 	CHECK_CALL(SetDACPowered(true, OutputModes::All, OutputResistances::R1K5));

	// 	CHECK_CALL(SetInputMute(false));
	// 	CHECK_CALL(SetOutputMute(false));

	// 	CHECK_CALL(SetADCEnabled(false));
	// 	CHECK_CALL(SetDACEnabled(false));

	// 	return true;
	// }

	static bool SetADCPowered(bool Powered, InputModes InputMode, MonoMixModes MonoMixMode)
	{
		Log::WriteDebug(TAG, "Setting ADC Powered: %i, Left1: %i, Right1: %i, Left2: %i, Right2: %i, Left1 and Right1 Diffrential: %i, Left2 and Right2 Diffrential: %i, Mono Mix to Left: %i, Mono Mix to Right: %i", Powered,
						Bitwise::IsEnabled(InputMode, InputModes::Left1),
						Bitwise::IsEnabled(InputMode, InputModes::Right1),
						Bitwise::IsEnabled(InputMode, InputModes::Left2),
						Bitwise::IsEnabled(InputMode, InputModes::Right2),
						Bitwise::IsEnabled(InputMode, InputModes::Left1AndRight1Differential),
						Bitwise::IsEnabled(InputMode, InputModes::Left2AndRight2Differential),
						Bitwise::IsEnabled(MonoMixMode, MonoMixModes::MonoMixToLeft),
						Bitwise::IsEnabled(MonoMixMode, MonoMixModes::MonoMixToRight));

		ES8388Control::Values adcPower = ES8388Control::Values::ADCPower_int1LP_1;
		ES8388Control::Values adcFlashPower = ES8388Control::Values::ADCPower_flashLP_1;
		ES8388Control::Values biasGeneratorPower = ES8388Control::Values::ADCPower_PdnADCBiasgen_1;
		if (Powered)
		{
			adcPower = ES8388Control::Values::ADCPower_int1LP_0;
			adcFlashPower = ES8388Control::Values::ADCPower_flashLP_0;
			biasGeneratorPower = ES8388Control::Values::ADCPower_PdnADCBiasgen_0;
		}

		ES8388Control::Write(ES8388Control::Registers::ADCPower, adcPower, ES8388Control::Masks::ADCPower_int1LP);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, adcFlashPower, ES8388Control::Masks::ADCPower_flashLP);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, biasGeneratorPower, ES8388Control::Masks::ADCPower_PdnADCBiasgen);

		ES8388Control::Write(ES8388Control::Registers::ADCControl2, ES8388Control::Values::ADCControl2_DSSEL_1, ES8388Control::Masks::ADCControl2_DSSEL);

		ES8388Control::Values leftADCPower = ES8388Control::Values::ADCPower_PdnADCL_1;
		ES8388Control::Values leftAnalogInputPower = ES8388Control::Values::ADCPower_PdnAINL_1;
		ES8388Control::Values lefInSelect = ES8388Control::Values::ADCControl2_LINSEL_00;
		ES8388Control::Values leftDiffrentialInputSelect = ES8388Control::Values::ADCControl3_DS_0;

		if (Powered && (Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Left2)))
		{
			leftADCPower = ES8388Control::Values::ADCPower_PdnADCL_0;
			leftAnalogInputPower = ES8388Control::Values::ADCPower_PdnAINL_0;

			if (Bitwise::IsEnabled(InputMode, InputModes::Left1AndRight1Differential))
			{
				lefInSelect = ES8388Control::Values::ADCControl2_LINSEL_11;
				leftDiffrentialInputSelect = ES8388Control::Values::ADCControl3_DS_0;
			}
			else if (Bitwise::IsEnabled(InputMode, InputModes::Left2AndRight2Differential))
			{
				lefInSelect = ES8388Control::Values::ADCControl2_LINSEL_11;
				leftDiffrentialInputSelect = ES8388Control::Values::ADCControl3_DS_1;
			}
			else if (Bitwise::IsEnabled(InputMode, InputModes::Left1))
				lefInSelect = ES8388Control::Values::ADCControl2_LINSEL_00;
			else if (Bitwise::IsEnabled(InputMode, InputModes::Left2))
				lefInSelect = ES8388Control::Values::ADCControl2_LINSEL_01;
		}

		ES8388Control::Write(ES8388Control::Registers::ADCPower, leftADCPower, ES8388Control::Masks::ADCPower_PdnADCL);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, leftAnalogInputPower, ES8388Control::Masks::ADCPower_PdnAINL);
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, lefInSelect, ES8388Control::Masks::ADCControl2_LINSEL);
		ES8388Control::Write(ES8388Control::Registers::ADCControl3, leftDiffrentialInputSelect, ES8388Control::Masks::ADCControl3_DS);

		ES8388Control::Values rightADCPower = ES8388Control::Values::ADCPower_PdnADCR_1;
		ES8388Control::Values rightAnalogInputPower = ES8388Control::Values::ADCPower_PdnAINR_1;
		ES8388Control::Values rightInSelect = ES8388Control::Values::ADCControl2_RINSEL_00;
		ES8388Control::Values rightDiffrentialInputSelect = ES8388Control::Values::ADCControl2_DSR_0;

		if (Powered && (Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Right2)))
		{
			rightADCPower = ES8388Control::Values::ADCPower_PdnADCR_0;
			rightAnalogInputPower = ES8388Control::Values::ADCPower_PdnAINR_0;

			if (Bitwise::IsEnabled(InputMode, InputModes::Left1AndRight1Differential))
			{
				rightInSelect = ES8388Control::Values::ADCControl2_RINSEL_11;
				rightDiffrentialInputSelect = ES8388Control::Values::ADCControl2_DSR_0;
			}
			else if (Bitwise::IsEnabled(InputMode, InputModes::Left2AndRight2Differential))
			{
				rightInSelect = ES8388Control::Values::ADCControl2_RINSEL_11;
				rightDiffrentialInputSelect = ES8388Control::Values::ADCControl2_DSR_1;
			}
			else if (Bitwise::IsEnabled(InputMode, InputModes::Right1))
				rightInSelect = ES8388Control::Values::ADCControl2_RINSEL_00;
			else if (Bitwise::IsEnabled(InputMode, InputModes::Right2))
				rightInSelect = ES8388Control::Values::ADCControl2_RINSEL_01;
		}

		ES8388Control::Write(ES8388Control::Registers::ADCPower, rightADCPower, ES8388Control::Masks::ADCPower_PdnADCR);
		ES8388Control::Write(ES8388Control::Registers::ADCPower, rightAnalogInputPower, ES8388Control::Masks::ADCPower_PdnAINR);
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, rightInSelect, ES8388Control::Masks::ADCControl2_RINSEL);
		ES8388Control::Write(ES8388Control::Registers::ADCControl2, rightDiffrentialInputSelect, ES8388Control::Masks::ADCControl2_DSR);

		ES8388Control::Values micBiasGenerator = ES8388Control::Values::ADCPower_PdnMICB_1;

		if (Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1))
			micBiasGenerator = ES8388Control::Values::ADCPower_PdnMICB_0;

		ES8388Control::Write(ES8388Control::Registers::ADCPower, micBiasGenerator, ES8388Control::Masks::ADCPower_PdnMICB);

		ES8388Control::Values monoMix = ES8388Control::Values::ADCControl3_MONOMIX_00;
		ES8388Control::Values monoMixDataSelect = ES8388Control::Values::ADCControl4_DATSEL_00;

		if (Powered)
		{
			if (Bitwise::IsEnabled(MonoMixMode, MonoMixModes::MonoMixToLeft))
			{
				monoMix = ES8388Control::Values::ADCControl3_MONOMIX_01;
				monoMixDataSelect = ES8388Control::Values::ADCControl4_DATSEL_01;
			}
			else if (Bitwise::IsEnabled(MonoMixMode, MonoMixModes::MonoMixToRight))
			{
				monoMix = ES8388Control::Values::ADCControl3_MONOMIX_10;
				monoMixDataSelect = ES8388Control::Values::ADCControl4_DATSEL_10;
			}
			else
			{
				monoMix = ES8388Control::Values::ADCControl3_MONOMIX_00;
				monoMixDataSelect = ES8388Control::Values::ADCControl4_DATSEL_00;
			}
		}

		ES8388Control::Write(ES8388Control::Registers::ADCControl3, monoMix, ES8388Control::Masks::ADCControl3_MONOMIX);
		ES8388Control::Write(ES8388Control::Registers::ADCControl4, monoMixDataSelect, ES8388Control::Masks::ADCControl4_DATSEL);

		return true;
	}

	static bool SetDACPowered(bool Powered, OutputModes OutputMode, OutputResistances OutputResistance)
	{
		Log::WriteDebug(TAG, "Setting DAC Powered: %i, Left1: %i, Right1: %i, Left2: %i, Right2: %i, Output Resistance: %i", Powered,
						Bitwise::IsEnabled(OutputMode, OutputModes::Left1),
						Bitwise::IsEnabled(OutputMode, OutputModes::Right1),
						Bitwise::IsEnabled(OutputMode, OutputModes::Left2),
						Bitwise::IsEnabled(OutputMode, OutputModes::Right2),
						OutputResistance);

		ES8388Control::Values leftDACPower = ES8388Control::Values::DACPower_PdnDACL_1;
		ES8388Control::Values rightDACPower = ES8388Control::Values::DACPower_PdnDACR_1;
		if (Powered)
		{
			if (Bitwise::IsEnabled(OutputMode, OutputModes::Left1) || Bitwise::IsEnabled(OutputMode, OutputModes::Left2))
				leftDACPower = ES8388Control::Values::DACPower_PdnDACL_0;

			if (Bitwise::IsEnabled(OutputMode, OutputModes::Right1) || Bitwise::IsEnabled(OutputMode, OutputModes::Right2))
				leftDACPower = ES8388Control::Values::DACPower_PdnDACR_0;
		}

		ES8388Control::Write(ES8388Control::Registers::DACPower, leftDACPower, ES8388Control::Masks::DACPower_PdnDACL);
		ES8388Control::Write(ES8388Control::Registers::DACPower, leftDACPower, ES8388Control::Masks::DACPower_PdnDACR);

		ES8388Control::Values leftOut1 = ES8388Control::Values::DACPower_LOUT1_0;
		ES8388Control::Values rightOut1 = ES8388Control::Values::DACPower_ROUT1_0;
		ES8388Control::Values leftOut2 = ES8388Control::Values::DACPower_LOUT2_0;
		ES8388Control::Values rightOut2 = ES8388Control::Values::DACPower_ROUT2_0;

		if (Powered)
		{
			if (Bitwise::IsEnabled(OutputMode, OutputModes::Left1))
				leftOut1 = ES8388Control::Values::DACPower_LOUT1_1;

			if (Bitwise::IsEnabled(OutputMode, OutputModes::Right1))
				rightOut1 = ES8388Control::Values::DACPower_ROUT1_1;

			if (Bitwise::IsEnabled(OutputMode, OutputModes::Left2))
				leftOut2 = ES8388Control::Values::DACPower_LOUT2_1;

			if (Bitwise::IsEnabled(OutputMode, OutputModes::Right2))
				rightOut2 = ES8388Control::Values::DACPower_ROUT2_1;
		}

		ES8388Control::Write(ES8388Control::Registers::DACPower, leftOut1, ES8388Control::Masks::DACPower_LOUT1);
		ES8388Control::Write(ES8388Control::Registers::DACPower, rightOut1, ES8388Control::Masks::DACPower_ROUT1);
		ES8388Control::Write(ES8388Control::Registers::DACPower, leftOut2, ES8388Control::Masks::DACPower_LOUT2);
		ES8388Control::Write(ES8388Control::Registers::DACPower, rightOut2, ES8388Control::Masks::DACPower_ROUT2);

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
			Log::WriteDebug(TAG, "Setting ADC Format: I2S");
			break;

		case Formats::LeftJustify:
			value = ES8388Control::Values::ADCControl4_ADCFORMAT_01;
			Log::WriteDebug(TAG, "Setting ADC Format: LeftJustify");
			break;

		case Formats::RightJustify:
			value = ES8388Control::Values::ADCControl4_ADCFORMAT_10;
			Log::WriteDebug(TAG, "Setting ADC Format: RightJustify");
			break;

		case Formats::DSP_PCM:
			value = ES8388Control::Values::ADCControl4_ADCFORMAT_11;
			Log::WriteDebug(TAG, "Setting ADC Format: DSP_PCM");
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
			Log::WriteDebug(TAG, "Setting DAC Format: I2S");
			break;

		case Formats::LeftJustify:
			value = ES8388Control::Values::DACControl1_DACFORMAT_01;
			Log::WriteDebug(TAG, "Setting DAC Format: LeftJustify");
			break;

		case Formats::RightJustify:
			value = ES8388Control::Values::DACControl1_DACFORMAT_10;
			Log::WriteDebug(TAG, "Setting DAC Format: RightJustify");
			break;

		case Formats::DSP_PCM:
			value = ES8388Control::Values::DACControl1_DACFORMAT_11;
			Log::WriteDebug(TAG, "Setting DAC Format: DSP_PCM");
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

		Log::WriteDebug(TAG, "Setting I2S ADC Bits Per Sample: %i", BPS);

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

		Log::WriteDebug(TAG, "Setting I2S DAC Bits Per Sample: %i", BPS);

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

	static bool SetAutomaticLevelControlEnabled(InputModes InputMode, bool Enabled)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Left2) || Bitwise::IsEnabled(InputMode, InputModes::Right2)))
			return false;

		Log::WriteDebug(TAG, "Setting ALC Enabled: %i, Right: %i, Left: %i", Enabled,
						Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Right2),
						Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Left2));

		ES8388Control::Values value = ES8388Control::Values::ADCControl10_ALCSEL_00;
		if (Enabled)
		{
			if (Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Right2))
				value |= ES8388Control::Values::ADCControl10_ALCSEL_01;

			if (Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Left2))
				value |= ES8388Control::Values::ADCControl10_ALCSEL_10;
		}

		ES8388Control::Write(ES8388Control::Registers::ADCControl10, value, ES8388Control::Masks::ADCControl10_ALCSEL);

		return true;
	}

	// dBMin [-12dB, 30dB]
	// dBMax [-6.5dB, 35.5dB]
	// dBTarget [-16.5dB, -1.5dB]
	// HoldTime [0ms, 1360ms]
	// AttackTime [0.104ms/0.0227ms, 106ms/23.2ms]
	// DecayTime [0.410ms/0.0908ms, 420ms/93ms]
	// WindowsSize [96, 496]
	static bool SetAutomaticLevelControlParameters(InputModes InputMode, float dBMin, float dBMax, float dBTarget, float HoldTime, float AttackTime, float DecayTime, uint8 WindowsSize, bool ZeroCrossTimeout, bool UseZeroCrossDetection, bool LimiterMode)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Left2) || Bitwise::IsEnabled(InputMode, InputModes::Right2)))
			return false;

		ASSERT(-12 <= dBMin && dBMin <= 30, "Invalid dBMin");
		ASSERT(-6.5F <= dBMax && dBMax <= 35.5F, "Invalid dBMax");
		ASSERT(-16.5F <= dBTarget && dBTarget <= -1.5F, "Invalid dBTarget");
		ASSERT(0 <= HoldTime && HoldTime <= 1360, "Invalid HoldTime");
		ASSERT(0.104 <= AttackTime && AttackTime <= 106, "Invalid AttackTime");
		ASSERT(0.410 <= DecayTime && DecayTime <= 420, "Invalid DecayTime");
		ASSERT(96 <= WindowsSize && WindowsSize <= 496, "Invalid WindowsSize");

		Log::WriteDebug(TAG, "Setting Automatic Level Control Parameters, Min: %.1fdB, Max: %.1fdB, Target: %.1fdB, Hold Time: %.1fms, Attack Time: %.1fms, Decay Time: %.1fms", dBMin, dBMax, dBTarget, HoldTime, AttackTime, DecayTime);

		uint8 value = (dBMin + 12) / 6;
		ES8388Control::Write(ES8388Control::Registers::ADCControl10, (ES8388Control::Values)value, ES8388Control::Masks::ADCControl10_MINGAIN);

		value = (dBMax + 6.5F) / 6;
		ES8388Control::Write(ES8388Control::Registers::ADCControl10, (ES8388Control::Values)(value << 3), ES8388Control::Masks::ADCControl10_MAXGAIN);

		value = (dBTarget + 16.5F) / 1.5F;
		ES8388Control::Write(ES8388Control::Registers::ADCControl11, (ES8388Control::Values)(value << 4), ES8388Control::Masks::ADCControl11_ALCLVL);

		value = log2(((HoldTime + 1000) / 0.67F) - 1000);
		ES8388Control::Write(ES8388Control::Registers::ADCControl11, (ES8388Control::Values)value, ES8388Control::Masks::ADCControl11_ALCHLD);

		value = log2(AttackTime / 0.053F);
		ES8388Control::Write(ES8388Control::Registers::ADCControl12, (ES8388Control::Values)value, ES8388Control::Masks::ADCControl12_ALCATK);

		value = log2(DecayTime / 0.21F);
		ES8388Control::Write(ES8388Control::Registers::ADCControl12, (ES8388Control::Values)(value << 4), ES8388Control::Masks::ADCControl12_ALCDCY);

		value = WindowsSize / 16;
		ES8388Control::Write(ES8388Control::Registers::ADCControl13, (ES8388Control::Values)value, ES8388Control::Masks::ADCControl13_WIN_SIZE);

		ES8388Control::Write(ES8388Control::Registers::ADCControl13, (ZeroCrossTimeout ? ES8388Control::Values::ADCControl13_TIME_OUT_1 : ES8388Control::Values::ADCControl13_TIME_OUT_0), ES8388Control::Masks::ADCControl13_TIME_OUT);
		ES8388Control::Write(ES8388Control::Registers::ADCControl13, (UseZeroCrossDetection ? ES8388Control::Values::ADCControl13_ALCZC_1 : ES8388Control::Values::ADCControl13_ALCZC_0), ES8388Control::Masks::ADCControl13_ALCZC);
		ES8388Control::Write(ES8388Control::Registers::ADCControl13, (LimiterMode ? ES8388Control::Values::ADCControl13_ALCMODE_1 : ES8388Control::Values::ADCControl13_ALCMODE_0), ES8388Control::Masks::ADCControl13_ALCMODE);

		return true;
	}

	static bool SetNoiseGateEnabled(InputModes InputMode, bool Enabled)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Left2) || Bitwise::IsEnabled(InputMode, InputModes::Right2)))
			return 0;

		Log::WriteDebug(TAG, "Setting Noise Gate Enabled: %i", Enabled);

		ES8388Control::Write(ES8388Control::Registers::ADCControl14, (Enabled ? ES8388Control::Values::ADCControl14_NGAT_1 : ES8388Control::Values::ADCControl14_NGAT_0), ES8388Control::Masks::ADCControl14_NGAT);

		return true;
	}

	//[-76.5dBFS, -30dBFS]
	static bool SetNoiseGateParameters(InputModes InputMode, float dBFS, bool MuteOnNoise)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Left2) || Bitwise::IsEnabled(InputMode, InputModes::Right2)))
			return 0;

		ASSERT(-76.5F <= dBFS && dBFS <= -30, "Invalid dBFS");

		Log::WriteDebug(TAG, "Setting Noise Gate: %.1fdBFS, Mute On Noise: %i", dBFS, MuteOnNoise);

		uint8 value = (dBFS + 76.5F) / -1.5;

		ES8388Control::Write(ES8388Control::Registers::ADCControl14, (ES8388Control::Values)(value << 3), ES8388Control::Masks::ADCControl14_NGTH);

		ES8388Control::Write(ES8388Control::Registers::ADCControl14, (MuteOnNoise ? ES8388Control::Values::ADCControl14_NGG_01 : ES8388Control::Values::ADCControl14_NGG_00), ES8388Control::Masks::ADCControl14_NGG);

		return true;
	}

	//[0dB, 24dB]
	static bool SetMicrophoneGain(InputModes InputMode, float dB)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1)))
			return false;

		ASSERT(0 <= dB && dB <= 24, "Invalid dB");

		Log::WriteDebug(TAG, "Setting Microphone Gain: %.1fdB", dB);

		uint8 value = (uint8)(dB / 3);

		if (Bitwise::IsEnabled(InputMode, InputModes::Left1))
			ES8388Control::Write(ES8388Control::Registers::ADCControl1, (ES8388Control::Values)((value << 4) & (uint8)ES8388Control::Masks::ADCControl1_MicAmpL), ES8388Control::Masks::ADCControl1_MicAmpL);

		if (Bitwise::IsEnabled(InputMode, InputModes::Right1))
			ES8388Control::Write(ES8388Control::Registers::ADCControl1, (ES8388Control::Values)(value & (uint8)ES8388Control::Masks::ADCControl1_MicAmpR), ES8388Control::Masks::ADCControl1_MicAmpR);

		return true;
	}

	static float GetMicrophoneGain(InputModes InputMode)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1)))
			return 0;

		ES8388Control::Values value = (ES8388Control::Values)0;

		if (Bitwise::IsEnabled(InputMode, InputModes::Left1))
			value = (ES8388Control::Values)((uint8)ES8388Control::Read(ES8388Control::Registers::ADCControl1, ES8388Control::Masks::ADCControl1_MicAmpR) >> 4);

		if (Bitwise::IsEnabled(InputMode, InputModes::Right1))
			value = ES8388Control::Read(ES8388Control::Registers::ADCControl1, ES8388Control::Masks::ADCControl1_MicAmpR);

		return (uint8)value * 3;
	}

	//[-15dB, 6dB]
	static bool SetInputToMixerGain(InputModes InputMode, float dB)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Left2) || Bitwise::IsEnabled(InputMode, InputModes::Right2)))
			return false;

		ASSERT(-15 <= dB && dB <= 6, "Invalid dB");

		Log::WriteDebug(TAG, "Setting Input to Mixer Gain: %.1fdB", dB);

		uint8 value = 7 - (uint8)((dB + 15) / 3);

		if (Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Left2))
			ES8388Control::Write(ES8388Control::Registers::DACControl17, (ES8388Control::Values)(value << 3), ES8388Control::Masks::DACControl17_LI2LOVOL);

		if (Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Right2))
			ES8388Control::Write(ES8388Control::Registers::DACControl20, (ES8388Control::Values)(value << 3), ES8388Control::Masks::DACControl20_RI2ROVOL);

		return true;
	}

	static float GetInputToMixerGain(InputModes InputMode)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Left2) || Bitwise::IsEnabled(InputMode, InputModes::Right2)))
			return 0;

		ES8388Control::Values value = (ES8388Control::Values)0;

		if (Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Left2))
			value = ES8388Control::Read(ES8388Control::Registers::DACControl17, ES8388Control::Masks::DACControl17_LI2LOVOL);

		if (Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Right2))
			value = ES8388Control::Read(ES8388Control::Registers::DACControl20, ES8388Control::Masks::DACControl20_RI2ROVOL);

		return ((7 - ((uint8)value >> 3)) * 3) - 15;
	}

	//[-96dB, 0dB]
	static bool SetInputVolume(InputModes InputMode, float dB)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Left2) || Bitwise::IsEnabled(InputMode, InputModes::Right2)))
			return false;

		ASSERT(-96 <= dB && dB <= 0, "Invalid dB");

		Log::WriteDebug(TAG, "Setting Input Volume: %.1fdB", dB);

		ES8388Control::Values value = (ES8388Control::Values)(((uint8)(dB * -2)) & (uint8)ES8388Control::Masks::ADCControl8_LADCVOL);

		if (Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Left2))
			ES8388Control::Write(ES8388Control::Registers::ADCControl8, value, ES8388Control::Masks::ADCControl8_LADCVOL);

		if (Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Right2))
			ES8388Control::Write(ES8388Control::Registers::ADCControl9, value, ES8388Control::Masks::ADCControl9_RADCVOL);

		return true;
	}

	static float GetInputVolume(InputModes InputMode)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Left2) || Bitwise::IsEnabled(InputMode, InputModes::Right2)))
			return 0;

		ES8388Control::Values value = (ES8388Control::Values)0;

		if (Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Left2))
			value = ES8388Control::Read(ES8388Control::Registers::ADCControl8, ES8388Control::Masks::ADCControl8_LADCVOL);

		if (Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Right2))
			value = ES8388Control::Read(ES8388Control::Registers::ADCControl9, ES8388Control::Masks::ADCControl9_RADCVOL);

		return (uint8)value / -2.0F;
	}

	static bool SetInputMute(InputModes InputMode, bool Mute)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Left2) || Bitwise::IsEnabled(InputMode, InputModes::Right2)))
			return false;

		Log::WriteDebug(TAG, "Setting Input Mute: %i", Mute);

		ES8388Control::Write(ES8388Control::Registers::ADCControl7, (Mute ? ES8388Control::Values::ADCControl7_ADCMute_1 : ES8388Control::Values::ADCControl7_ADCMute_0), ES8388Control::Masks::ADCControl7_ADCMute);

		return true;
	}

	static bool GetInputMute(InputModes InputMode)
	{
		if (!(Bitwise::IsEnabled(InputMode, InputModes::Left1) || Bitwise::IsEnabled(InputMode, InputModes::Right1) || Bitwise::IsEnabled(InputMode, InputModes::Left2) || Bitwise::IsEnabled(InputMode, InputModes::Right2)))
			return false;

		return (ES8388Control::Read(ES8388Control::Registers::ADCControl7, ES8388Control::Masks::ADCControl7_ADCMute) == ES8388Control::Values::ADCControl7_ADCMute_1);
	}

	//[-96dB, 0dB]
	static bool SetDigitalVolume(OutputModes OutputMode, float dB)
	{
		if (!(Bitwise::IsEnabled(OutputMode, OutputModes::Left1) || Bitwise::IsEnabled(OutputMode, OutputModes::Right1) || Bitwise::IsEnabled(OutputMode, OutputModes::Left2) || Bitwise::IsEnabled(OutputMode, OutputModes::Right2)))
			return false;

		ASSERT(-96 <= dB && dB <= 0, "Invalid dB");

		Log::WriteDebug(TAG, "Setting Digital Volume: %.1fdB", dB);

		ES8388Control::Values value = (ES8388Control::Values)(((uint8)(dB * -2)) & (uint8)ES8388Control::Masks::DACControl4_LDACVOL);

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Left1) || Bitwise::IsEnabled(OutputMode, OutputModes::Left2))
			ES8388Control::Write(ES8388Control::Registers::DACControl4, value, ES8388Control::Masks::DACControl4_LDACVOL);

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Right1) || Bitwise::IsEnabled(OutputMode, OutputModes::Right2))
			ES8388Control::Write(ES8388Control::Registers::DACControl5, value, ES8388Control::Masks::DACControl5_RDACVOL);

		return true;
	}

	static float GetDigitalVolume(OutputModes OutputMode)
	{
		if (!(Bitwise::IsEnabled(OutputMode, OutputModes::Left1) || Bitwise::IsEnabled(OutputMode, OutputModes::Right1) || Bitwise::IsEnabled(OutputMode, OutputModes::Left2) || Bitwise::IsEnabled(OutputMode, OutputModes::Right2)))
			return 0;

		ES8388Control::Values value = (ES8388Control::Values)0;

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Left1) || Bitwise::IsEnabled(OutputMode, OutputModes::Left2))
			value = ES8388Control::Read(ES8388Control::Registers::DACControl4, ES8388Control::Masks::DACControl4_LDACVOL);

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Right1) || Bitwise::IsEnabled(OutputMode, OutputModes::Right2))
			value = ES8388Control::Read(ES8388Control::Registers::DACControl5, ES8388Control::Masks::DACControl5_RDACVOL);

		return (uint8)value / -2.0F;
	}

	//[-45dB, 4.5dB]
	static bool SetOutputVolume(OutputModes OutputMode, float dB)
	{
		if (!(Bitwise::IsEnabled(OutputMode, OutputModes::Left1) || Bitwise::IsEnabled(OutputMode, OutputModes::Right1) || Bitwise::IsEnabled(OutputMode, OutputModes::Left2) || Bitwise::IsEnabled(OutputMode, OutputModes::Right2)))
			return 0;

		ASSERT(-45 <= dB && dB <= 4.5F, "Invalid dB");

		Log::WriteDebug(TAG, "Setting Output Volume: %.1fdB", dB);

		ES8388Control::Values value = (ES8388Control::Values)((uint8)((dB + 45) / 1.5F) & (uint8)ES8388Control::Masks::DACControl24_LOUT1VOL);

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Left1))
			ES8388Control::Write(ES8388Control::Registers::DACControl24, (ES8388Control::Values)value, ES8388Control::Masks::DACControl24_LOUT1VOL);

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Right1))
			ES8388Control::Write(ES8388Control::Registers::DACControl25, (ES8388Control::Values)value, ES8388Control::Masks::DACControl25_ROUT1VOL);

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Left2))
			ES8388Control::Write(ES8388Control::Registers::DACControl26, (ES8388Control::Values)value, ES8388Control::Masks::DACControl26_LOUT2VOL);

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Right2))
			ES8388Control::Write(ES8388Control::Registers::DACControl27, (ES8388Control::Values)value, ES8388Control::Masks::DACControl27_ROUT2VOL);

		return true;
	}

	static float GetOutputVolume(OutputModes OutputMode)
	{
		if (!(Bitwise::IsEnabled(OutputMode, OutputModes::Left1) || Bitwise::IsEnabled(OutputMode, OutputModes::Right1) || Bitwise::IsEnabled(OutputMode, OutputModes::Left2) || Bitwise::IsEnabled(OutputMode, OutputModes::Right2)))
			return 0;

		ES8388Control::Values value = (ES8388Control::Values)0;

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Left1))
			value = ES8388Control::Read(ES8388Control::Registers::DACControl24, ES8388Control::Masks::DACControl24_LOUT1VOL);

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Right1))
			value = ES8388Control::Read(ES8388Control::Registers::DACControl25, ES8388Control::Masks::DACControl25_ROUT1VOL);

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Left2))
			value = ES8388Control::Read(ES8388Control::Registers::DACControl26, ES8388Control::Masks::DACControl26_LOUT2VOL);

		if (Bitwise::IsEnabled(OutputMode, OutputModes::Right2))
			value = ES8388Control::Read(ES8388Control::Registers::DACControl27, ES8388Control::Masks::DACControl27_ROUT2VOL);

		return ((uint8)value * 1.5F) - 45;
	}

	static bool SetOutputMute(OutputModes OutputMode, bool Mute)
	{
		if (!(Bitwise::IsEnabled(OutputMode, OutputModes::Left1) || Bitwise::IsEnabled(OutputMode, OutputModes::Right1) || Bitwise::IsEnabled(OutputMode, OutputModes::Left2) || Bitwise::IsEnabled(OutputMode, OutputModes::Right2)))
			return 0;

		Log::WriteDebug(TAG, "Setting Output Mute: %i", Mute);

		ES8388Control::Write(ES8388Control::Registers::DACControl3, (Mute ? ES8388Control::Values::DACControl3_DACMute_1 : ES8388Control::Values::DACControl3_DACMute_0), ES8388Control::Masks::DACControl3_DACMute);

		return true;
	}

	static bool GetOutputMute(OutputModes OutputMode)
	{
		if (!(Bitwise::IsEnabled(OutputMode, OutputModes::Left1) || Bitwise::IsEnabled(OutputMode, OutputModes::Right1) || Bitwise::IsEnabled(OutputMode, OutputModes::Left2) || Bitwise::IsEnabled(OutputMode, OutputModes::Right2)))
			return 0;

		return (ES8388Control::Read(ES8388Control::Registers::DACControl3, ES8388Control::Masks::DACControl3_DACMute) == ES8388Control::Values::DACControl3_DACMute_1);
	}

private:
	static bool SetChipControls(MiddleVoltageResistances MiddleVoltageResistance, bool ResetControlPort, bool PoweredOn)
	{
		Log::WriteDebug(TAG, "Setting Chip Control Middle Voltage Resistance: %i, Reset Control Port: %i, Powered On: %i", MiddleVoltageResistance, ResetControlPort, PoweredOn);

		ES8388Control::Write(ES8388Control::Registers::ChipControl1, (ES8388Control::Values)MiddleVoltageResistance, ES8388Control::Masks::ChipControl1_VMIDSEL);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_EnRef_1, ES8388Control::Masks::ChipControl1_EnRef);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_SeqEn_0, ES8388Control::Masks::ChipControl1_SeqEn);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_SameFs_0, ES8388Control::Masks::ChipControl1_SameFs);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_DACMCLK_0, ES8388Control::Masks::ChipControl1_DACMCLK);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, ES8388Control::Values::ChipControl1_LRCM_0, ES8388Control::Masks::ChipControl1_LRCM);
		ES8388Control::Write(ES8388Control::Registers::ChipControl1, (ResetControlPort ? ES8388Control::Values::ChipControl1_SCPReset_1 : ES8388Control::Values::ChipControl1_SCPReset_0), ES8388Control::Masks::ChipControl1_SCPReset);

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
		Log::WriteDebug(TAG, "Setting ADC Enabled: %i, ChangeDLL: %i", Enabled, ChangeDLL);

		ES8388Control::Values analogReferencePower = ES8388Control::Values::ChipPower_adcVref_PDN_1;
		ES8388Control::Values dllPower = ES8388Control::Read(ES8388Control::Registers::ChipPower, ES8388Control::Masks::ChipPower_ADCDLL_PDN);
		ES8388Control::Values resetStateMachine = ES8388Control::Values::ChipPower_adc_stm_rst_1;
		ES8388Control::Values resetDEM = ES8388Control::Values::ChipPower_adc_DigPDN_1;

		if (Enabled)
		{
			analogReferencePower = ES8388Control::Values::ChipPower_adcVref_PDN_0;

			if (ChangeDLL)
				dllPower = ES8388Control::Values::ChipPower_ADCDLL_PDN_0;

			resetStateMachine = ES8388Control::Values::ChipPower_adc_stm_rst_0;
			resetDEM = ES8388Control::Values::ChipPower_adc_DigPDN_0;
		}

		ES8388Control::Write(ES8388Control::Registers::ChipPower, analogReferencePower, ES8388Control::Masks::ChipPower_adcVref_PDN);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, dllPower, ES8388Control::Masks::ChipPower_ADCDLL_PDN);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, resetStateMachine, ES8388Control::Masks::ChipPower_adc_stm_rst);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, resetDEM, ES8388Control::Masks::ChipPower_adc_DigPDN);

		ES8388Control::Write(ES8388Control::Registers::ADCControl5, ES8388Control::Values::ADCControl5_ADCFsRatio_00010, ES8388Control::Masks::ADCControl5_ADCFsRatio);
		ES8388Control::Write(ES8388Control::Registers::ADCControl5, ES8388Control::Values::ADCControl5_ADCFsMode_0, ES8388Control::Masks::ADCControl5_ADCFsMode);

		return true;
	}

	static bool SetDACEnabled(bool Enabled, bool ChangeDLL = true)
	{
		Log::WriteDebug(TAG, "Setting DAC Enabled: %i, ChangeDLL: %i", Enabled, ChangeDLL);

		ES8388Control::Values analogReferencePower = ES8388Control::Values::ChipPower_dacVref_PDN_1;
		ES8388Control::Values dllPower = ES8388Control::Read(ES8388Control::Registers::ChipPower, ES8388Control::Masks::ChipPower_DACDLL_PDN);
		ES8388Control::Values resetStateMachine = ES8388Control::Values::ChipPower_dac_stm_rst_1;
		ES8388Control::Values resetDEM = ES8388Control::Values::ChipPower_dac_DigPDN_1;

		if (Enabled)
		{
			analogReferencePower = ES8388Control::Values::ChipPower_dacVref_PDN_0;

			if (ChangeDLL)
				dllPower = ES8388Control::Values::ChipPower_DACDLL_PDN_0;

			resetStateMachine = ES8388Control::Values::ChipPower_dac_stm_rst_0;
			resetDEM = ES8388Control::Values::ChipPower_dac_DigPDN_0;
		}

		ES8388Control::Write(ES8388Control::Registers::ChipPower, analogReferencePower, ES8388Control::Masks::ChipPower_dacVref_PDN);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, dllPower, ES8388Control::Masks::ChipPower_DACDLL_PDN);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, resetStateMachine, ES8388Control::Masks::ChipPower_dac_stm_rst);
		ES8388Control::Write(ES8388Control::Registers::ChipPower, resetDEM, ES8388Control::Masks::ChipPower_dac_DigPDN);

		ES8388Control::Write(ES8388Control::Registers::DACControl1, ES8388Control::Values::DACControl1_DACLRSWAP_0, ES8388Control::Masks::DACControl1_DACLRSWAP);

		ES8388Control::Write(ES8388Control::Registers::DACControl2, ES8388Control::Values::DACCOntrol2_DACFsRatio_00010, ES8388Control::Masks::DACCOntrol2_DACFsRatio);
		ES8388Control::Write(ES8388Control::Registers::DACControl2, ES8388Control::Values::DACCOntrol2_DACFsMode_0, ES8388Control::Masks::DACCOntrol2_DACFsMode);

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