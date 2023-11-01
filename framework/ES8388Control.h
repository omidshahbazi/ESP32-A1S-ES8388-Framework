#pragma once
#ifndef ES8388_CONTROL_H
#define ES8388_CONTROL_H

#include "I2CUtils.h"

// More info:
// https://dl.radxa.com/rock2/docs/hw/ds/ES8388%20user%20Guide.pdf
// https://docs.google.com/spreadsheets/d/1_UdXtrtpj_dzi36CSwv5tBphtGsBrY2Npdi79h1dYUk/edit#gid=0
class ES8388Control
{
public:
	enum class Registers
	{
		ChipControl1 = 0,
		ChipControl2 = 1,
		ChipPower = 2,
		ADCPower = 3,
		DACPower = 4,
		LowPower1 = 5,
		LowPower2 = 6,
		AnalogVoltage = 7,
		MasterMode = 8,

		ADCControl1 = 9,
		ADCControl2 = 10,
		ADCControl3 = 11,
		ADCControl4 = 12,
		ADCControl5 = 13,
		ADCControl6 = 14,
		ADCControl7 = 15,
		ADCControl8 = 16,
		ADCControl9 = 17,
		ADCControl10 = 18,
		ADCControl11 = 19,
		ADCControl12 = 20,
		ADCControl13 = 21,
		ADCControl14 = 22,

		DACControl1 = 23,
		DACControl2 = 24,
		DACControl3 = 25,
		DACControl4 = 26,
		DACControl5 = 27,
		DACControl6 = 28,
		DACControl7 = 29,
		DACControl8 = 30,
		DACControl9 = 31,
		DACControl10 = 32,
		DACControl11 = 33,
		DACControl12 = 34,
		DACControl13 = 35,
		DACControl14 = 36,
		DACControl15 = 37,
		DACControl16 = 38,
		DACControl17 = 39,
		DACControl18 = 40,
		DACControl19 = 41,
		DACControl20 = 42,
		DACControl21 = 43,
		DACControl22 = 44,
		DACControl23 = 45,
		DACControl24 = 46,
		DACControl25 = 47,
		DACControl26 = 48,
		DACControl27 = 49,
		DACControl28 = 50,
		DACControl29 = 51,
		DACControl30 = 52
	};

	enum class Masks
	{
		ChipControl1_VMIDSEL = 0b00000011,
		ChipControl1_EnRef = 0b00000100,
		ChipControl1_SeqEn = 0b00001000,
		ChipControl1_SameFs = 0b00010000,
		ChipControl1_DACMCLK = 0b00100000,
		ChipControl1_LRCM = 0b01000000,
		ChipControl1_SCPReset = 0b10000000,

		ChipControl2_PdnVrefbuf = 0b00000001,
		ChipControl2_VrefLo = 0b00000010,
		ChipControl2_PdnIbiasgen = 0b00000100,
		ChipControl2_PdnAna = 0b00001000,
		ChipControl2_LPVrefBuf = 0b00010000,
		ChipControl2_LPVcmMod = 0b00100000,

		ChipPower_dacVref_PDN = 0b00000001,
		ChipPower_adcVref_PDN = 0b00000010,
		ChipPower_DACDLL_PDN = 0b00000100,
		ChipPower_ADCDLL_PDN = 0b00001000,
		ChipPower_dac_stm_rst = 0b00010000,
		ChipPower_adc_stm_rst = 0b00100000,
		ChipPower_dac_DigPDN = 0b01000000,
		ChipPower_adc_DigPDN = 0b10000000,

		ADCPower_int1LP = 0b00000001,
		ADCPower_flashLP = 0b00000010,
		ADCPower_PdnADCBiasgen = 0b00000100,
		ADCPower_PdnMICB = 0b00001000,
		ADCPower_PdnADCR = 0b00010000,
		ADCPower_PdnADCL = 0b00100000,
		ADCPower_PdnAINR = 0b01000000,
		ADCPower_PdnAINL = 0b10000000,

		DACPower_ROUT2 = 0b00000100,
		DACPower_LOUT2 = 0b00001000,
		DACPower_ROUT1 = 0b00010000,
		DACPower_LOUT1 = 0b00100000,
		DACPower_PdnDACR = 0b01000000,
		DACPower_PdnDACL = 0b10000000,

		LowPower1_LPLOUT2 = 0b00001000,
		LowPower1_LPLOUT1 = 0b00100000,
		LowPower1_LPDACR = 0b01000000,
		LowPower1_LPDACL = 0b10000000,

		LowPower2_LPDACvrp = 0b00000001,
		LowPower2_LPADCvrp = 0b00000010,
		LowPower2_LPLMIX = 0b01000000,
		LowPower2_LPPGA = 0b10000000,

		AnalogVoltage_VSEL = 0b00111111,

		MasterMode_BCLKDIV = 0b00011111,
		MasterMode_BCLK_INV = 0b00100000,
		MasterMode_MCLKDIV2 = 0b01000000,
		MasterMode_MSC = 0b10000000,

		ADCControl1_MicAmpR = 0b00001111,
		ADCControl1_MicAmpL = 0b11110000,

		ADCControl2_DSR = 0b00000100,
		ADCControl2_DSSEL = 0b00001000,
		ADCControl2_RINSEL = 0b00110000,
		ADCControl2_LINSEL = 0b11000000,

		ADCControl3_TRI = 0b00000100,
		ADCControl3_MONOMIX = 0b00011000,
		ADCControl3_DS = 0b10000000,

		ADCControl4_ADCFORMAT = 0b00000011,
		ADCControl4_ADCWL = 0b00011100,
		ADCControl4_ADCLRP = 0b00100000,
		ADCControl4_DATSEL = 0b11000000,

		ADCControl5_ADCFsRatio = 0b00011111,
		ADCControl5_ADCFsMode = 0b00100000,

		ADCControl6_ADC_HPF_R = 0b00010000,
		ADCControl6_ADC_HPF_L = 0b00100000,
		ADCControl6_ADC_invR = 0b01000000,
		ADCControl6_ADC_invL = 0b10000000,

		ADCControl7_ADCMute = 0b00000100,
		ADCControl7_ADCLeR = 0b00001000,
		ADCControl7_ADCSoftRamp = 0b00100000,
		ADCControl7_ADCRampRate = 0b11000000,

		ADCControl8_LADCVOL = 0b11111111,

		ADCControl9_RADCVOL = 0b11111111,

		ADCControl10_MINGAIN = 0b00000111,
		ADCControl10_MAXGAIN = 0b00111000,
		ADCControl10_ALCSEL = 0b11000000,

		ADCControl11_ALCHLD = 0b00001111,
		ADCControl11_ALCLVL = 0b11110000,

		ADCControl12_ALCATK = 0b00001111,
		ADCControl12_ALCDCY = 0b11110000,

		ADCControl13_WIN_SIZE = 0b00011111,
		ADCControl13_TIME_OUT = 0b00100000,
		ADCControl13_ALCZC = 0b01000000,
		ADCControl13_ALCMODE = 0b10000000,

		ADCControl14_NGAT = 0b00000001,
		ADCControl14_NGG = 0b00000110,
		ADCControl14_NGTH = 0b11111000,

		DACControl1_DACFORMAT = 0b00000110,
		DACControl1_DACWL = 0b00111000,
		DACControl1_DACLRP = 0b01000000,
		DACControl1_DACLRSWAP = 0b10000000,

		DACCOntrol2_DACFsRatio = 0b00011111,
		DACCOntrol2_DACFsMode = 0b00100000,

		DACControl3_DACMute = 0b00000100,
		DACControl3_DACLeR = 0b00001000,
		DACControl3_DACSoftRamp = 0b00100000,
		DACControl3_DACRampRate = 0b11000000,

		DACControl4_LDACVOL = 0b11111111,

		DACControl5_RDACVOL = 0b11111111,

		DACControl6_DEEMP = 0b00001000,
		DACControl6_DAC_invR = 0b00010000,
		DACControl6_DAC_invL = 0b00100000,
		DACControl6_dacVref_PDN = 0b11000000,

		DACControl7_Vpp_scale = 0b00000011,
		DACControl7_SE = 0b00011100,
		DACControl7_Mono = 0b00100000,
		DACControl7_ZeroR = 0b01000000,
		DACControl7_ZeroL = 0b10000000,

		DACControl8_Shelving_a_24_29 = 0b00111111,
		DACControl9_Shelving_a_16_23 = 0b11111111,
		DACControl10_Shelving_a_8_15 = 0b11111111,
		DACControl11_Shelving_a_0_7 = 0b11111111,

		DACControl12_Shelving_b_24_29 = 0b00111111,
		DACControl13_Shelving_b_16_23 = 0b11111111,
		DACControl14_Shelving_b_8_15 = 0b11111111,
		DACControl15_Shelving_b_0_7 = 0b11111111,

		DACControl16_RMIXSEL = 0b00000111,
		DACControl16_LMIXSEL = 0b00111000,

		DACControl17_LI2LOVOL = 0b00111000,
		DACControl17_LI2LO = 0b01000000,
		DACControl17_LD2LO = 0b10000000,

		// Nothing for DACControl18
		// Nothing for DACControl19

		DACControl20_RI2ROVOL = 0b00111000,
		DACControl20_RI2RO = 0b01000000,
		DACControl20_RD2RO = 0b10000000,

		DACControl21_dac_dll_pwd = 0b00000100,
		DACControl21_adc_dll_pwd = 0b00001000,
		DACControl21_mclk_dis = 0b00010000,
		DACControl21_offset_dis = 0b00100000,
		DACControl21_lrck_sel = 0b01000000,
		DACControl21_slrck = 0b10000000,

		DACControl22_offset = 0b11111111,

		DACControl23_VROI = 0b00010000,

		DACControl24_LOUT1VOL = 0b00111111,

		DACControl25_ROUT1VOL = 0b00111111,

		DACControl26_LOUT2VOL = 0b00111111,

		DACControl27_ROUT2VOL = 0b00111111,

		// Nothing for DACControl28

		DACControl29_hpLout1_ref2 = 0b01000000,
		DACControl29_hpLout1_ref1 = 0b10000000,

		DACControl30_MREF2 = 0b00000001,
		DACControl30_MREF1 = 0b00000010,
		DACControl30_mixer_ref2 = 0b00000100,
		DACControl30_mixer_ref1 = 0b00001000,
		DACControl30_spkLout2_ref2 = 0b01000000,
		DACControl30_spkLout2_ref1 = 0b10000000
	};

	enum class Values
	{
		ChipControl1_VMIDSEL_00 = 0b00000000, // Vmid disabled
		ChipControl1_VMIDSEL_01 = 0b00000001, // 50 kΩ divider enabled
		ChipControl1_VMIDSEL_10 = 0b00000010, // 500 kΩ divider enabled (default)
		ChipControl1_VMIDSEL_11 = 0b00000011, // 5 kΩ divider enabled

		ChipControl1_EnRef_0 = 0b00000000, // disable reference
		ChipControl1_EnRef_1 = 0b00000100, // enable reference (default)

		ChipControl1_SeqEn_0 = 0b00000000, // internal power up/down sequence disable (default)
		ChipControl1_SeqEn_1 = 0b00001000, // internal power up/down sequence enable

		ChipControl1_SameFs_0 = 0b00000000, // ADC Fs differs from DAC Fs (default)
		ChipControl1_SameFs_1 = 0b00010000, // ADC Fs is the same as DAC Fs

		ChipControl1_DACMCLK_0 = 0b00000000, // when SameFs=1, ADCMCLK is the chip master clock source (default)
		ChipControl1_DACMCLK_1 = 0b00100000, // when SameFs=1, DACMCLK is the chip master clock source

		ChipControl1_LRCM_0 = 0b00000000, // ALRCK disabled when both ADC disabled; DLRCK disabled when both DAC disabled (default)
		ChipControl1_LRCM_1 = 0b01000000, // ALRCK and DLRCK disabled when all ADC and DAC disabled

		ChipControl1_SCPReset_0 = 0b00000000, // normal (default)
		ChipControl1_SCPReset_1 = 0b10000000, // reset control port register to default

		ChipControl2_PdnVrefbuf_0 = 0b00000000, // normal (default)
		ChipControl2_PdnVrefbuf_1 = 0b00000001, // power down

		ChipControl2_VrefLo_0 = 0b00000000, // normal (default)
		ChipControl2_VrefLo_1 = 0b00000010, // low power

		ChipControl2_PdnIbiasgen_0 = 0b00000000, // normal
		ChipControl2_PdnIbiasgen_1 = 0b00000100, // ibiasgen power down (default)

		ChipControl2_PdnAna_0 = 0b00000000, // normal
		ChipControl2_PdnAna_1 = 0b00001000, // entire analog power down (default)

		ChipControl2_LPVrefBuf_0 = 0b00000000, // normal
		ChipControl2_LPVrefBuf_1 = 0b00010000, // low power (default)

		ChipControl2_LPVcmMod_0 = 0b00000000, // normal (default)
		ChipControl2_LPVcmMod_1 = 0b00100000, // low power

		ChipPower_dacVref_PDN_0 = 0b00000000, // DAC analog reference power up
		ChipPower_dacVref_PDN_1 = 0b00000001, // DAC analog reference power down (default)

		ChipPower_adcVref_PDN_0 = 0b00000000, // ADC analog reference power up
		ChipPower_adcVref_PDN_1 = 0b00000010, // ADC analog reference power down (default)

		ChipPower_DACDLL_PDN_0 = 0b00000000, // normal (default)
		ChipPower_DACDLL_PDN_1 = 0b00000100, // DAC DLL power down, stop DAC clock

		ChipPower_ADCDLL_PDN_0 = 0b00000000, // normal (default)
		ChipPower_ADCDLL_PDN_1 = 0b00001000, // ADC_DLL power down, stop ADC clock

		ChipPower_dac_stm_rst_0 = 0b00000000, // normal (default)
		ChipPower_dac_stm_rst_1 = 0b00010000, // reset DAC state machine to power down state

		ChipPower_adc_stm_rst_0 = 0b00000000, // normal (default)
		ChipPower_adc_stm_rst_1 = 0b00100000, // reset ADC state machine to power down state

		ChipPower_dac_DigPDN_0 = 0b00000000, // normal
		ChipPower_dac_DigPDN_1 = 0b01000000, // resets DAC DSM, DEM, filter and serial data port (default)

		ChipPower_adc_DigPDN_0 = 0b00000000, // normal
		ChipPower_adc_DigPDN_1 = 0b10000000, // resets ADC DEM, filter and serial data port (default)

		ADCPower_int1LP_0 = 0b00000000, // normal (default)
		ADCPower_int1LP_1 = 0b00000001, // int1 low power

		ADCPower_flashLP_0 = 0b00000000, // normal (default)
		ADCPower_flashLP_1 = 0b00000010, // flash ADC low power

		ADCPower_PdnADCBiasgen_0 = 0b00000000, // normal
		ADCPower_PdnADCBiasgen_1 = 0b00000100, // power down (default)

		ADCPower_PdnMICB_0 = 0b00000000, // microphone bias power on
		ADCPower_PdnMICB_1 = 0b00001000, // microphone bias power down (high impedance output, default)

		ADCPower_PdnADCR_0 = 0b00000000, // right ADC power up
		ADCPower_PdnADCR_1 = 0b00010000, // right ADC power down (default)

		ADCPower_PdnADCL_0 = 0b00000000, // left ADC power up
		ADCPower_PdnADCL_1 = 0b00100000, // left ADC power down (default)

		ADCPower_PdnAINR_0 = 0b00000000, // normal
		ADCPower_PdnAINR_1 = 0b01000000, // right analog input power down (default)

		ADCPower_PdnAINL_0 = 0b00000000, // normal
		ADCPower_PdnAINL_1 = 0b10000000, // left analog input power down (default)

		DACPower_ROUT2_0 = 0b00000000, // ROUT2 disabled (default)
		DACPower_ROUT2_1 = 0b00000100, // ROUT2 enabled

		DACPower_LOUT2_0 = 0b00000000, // LOUT2 disabled (default)
		DACPower_LOUT2_1 = 0b00001000, // LOUT2 enabled

		DACPower_ROUT1_0 = 0b00000000, // ROUT1 disabled (default)
		DACPower_ROUT1_1 = 0b00010000, // ROUT1 enabled

		DACPower_LOUT1_0 = 0b00000000, // LOUT1 disabled (default)
		DACPower_LOUT1_1 = 0b00100000, // LOUT1 enabled

		DACPower_PdnDACR_0 = 0b00000000, // right DAC power up
		DACPower_PdnDACR_1 = 0b01000000, // right DAC power down (default)

		DACPower_PdnDACL_0 = 0b00000000, // left DAC power up
		DACPower_PdnDACL_1 = 0b10000000, // left DAC power down (default)

		LowPower1_LPLOUT2_0 = 0b00000000, // normal (default)
		LowPower1_LPLOUT2_1 = 0b00001000, // low power

		LowPower1_LPLOUT1_0 = 0b00000000, // normal (default)
		LowPower1_LPLOUT1_1 = 0b00100000, // low power

		LowPower1_LPDACR_0 = 0b00000000, // normal (default)
		LowPower1_LPDACR_1 = 0b01000000, // low power

		LowPower1_LPDACL_0 = 0b00000000, // normal (default)
		LowPower1_LPDACL_1 = 0b10000000, // low power

		LowPower2_LPDACvrp_0 = 0b00000000, // normal (default)
		LowPower2_LPDACvrp_1 = 0b00000001, // low power

		LowPower2_LPADCvrp_0 = 0b00000000, // normal (default)
		LowPower2_LPADCvrp_1 = 0b00000010, // low power

		LowPower2_LPLMIX_0 = 0b00000000, // normal (default)
		LowPower2_LPLMIX_1 = 0b01000000, // low power

		LowPower2_LPPGA_0 = 0b00000000, // normal (default)
		LowPower2_LPPGA_1 = 0b10000000, // low power

		AnalogVoltage_VSEL_11111100 = 0b11111100, // normal (default)

		MasterMode_BCLKDIV_00000 = 0b00000000,	// master mode BCLK generated automatically based on the clock table (default)
		MasterMode_BCLKDIV_00001 = 0b00000001,	// MCLK/1
		MasterMode_BCLKDIV_00010 = 0b00000010,	// MCLK/2
		MasterMode_BCLKDIV_00011 = 0b00000011,	// MCLK/3
		MasterMode_BCLKDIV_00100 = 0b00000100,	// MCLK/4
		MasterMode_BCLKDIV_00101 = 0b00000101,	// MCLK/6
		MasterMode_BCLKDIV_00110 = 0b00000110,	// MCLK/8
		MasterMode_BCLKDIV_00111 = 0b00000111,	// MCLK/9
		MasterMode_BCLKDIV_01000 = 0b00001000,	// MCLK/11
		MasterMode_BCLKDIV_01001 = 0b00001001,	// MCLK/12
		MasterMode_BCLKDIV_01010 = 0b00001010,	// MCLK/16
		MasterMode_BCLKDIV_01011 = 0b00001011,	// MCLK/18
		MasterMode_BCLKDIV_01100 = 0b00001100,	// MCLK/22
		MasterMode_BCLKDIV_01101 = 0b00001101,	// MCLK/24
		MasterMode_BCLKDIV_01110 = 0b00001110,	// MCLK/33
		MasterMode_BCLKDIV_01111 = 0b00001111,	// MCLK/36
		MasterMode_BCLKDIV_10000 = 0b00010000,	// MCLK/44
		MasterMode_BCLKDIV_10001 = 0b00010001,	// MCLK/48
		MasterMode_BCLKDIV_10010 = 0b00010010,	// MCLK/66
		MasterMode_BCLKDIV_10011 = 0b00010011,	// MCLK/72
		MasterMode_BCLKDIV_10100 = 0b00010100,	// MCLK/5
		MasterMode_BCLKDIV_10101 = 0b00010101,	// MCLK/10
		MasterMode_BCLKDIV_10110 = 0b00010110,	// MCLK/15
		MasterMode_BCLKDIV_10111 = 0b00010111,	// MCLK/17
		MasterMode_BCLKDIV_11000 = 0b00011000,	// MCLK/20
		MasterMode_BCLKDIV_11001 = 0b00011001,	// MCLK/25
		MasterMode_BCLKDIV_11010 = 0b00011010,	// MCLK/30
		MasterMode_BCLKDIV_11011 = 0b00011011,	// MCLK/32
		MasterMode_BCLKDIV_11100 = 0b00011100,	// MCLK/34
		MasterMode_BCLKDIV_Others = 0b00011111, // MCLK/4

		MasterMode_BCLK_INV_0 = 0b00000000, // normal (default)
		MasterMode_BCLK_INV_1 = 0b00100000, // BCLK inverted

		MasterMode_MCLKDIV2_0 = 0b00000000, // MCLK not divide (default)
		MasterMode_MCLKDIV2_1 = 0b01000000, // MCLK divide by 2

		MasterMode_MSC_0 = 0b00000000, // slave serial port mode
		MasterMode_MSC_1 = 0b10000000, // master serial port mode (default)

		ADCControl1_MicAmpR_0000 = 0b00000000, // Right channel PGA gain, 0dB (default), Floor((Expected dB between [0dB, 24dB]) / 3dB) would be the interpolated value between these two
		ADCControl1_MicAmpR_1000 = 0b00001000, // Right channel PGA gain, 24dB, Floor((Expected dB between [0dB, 24dB]) / 3dB) would be the interpolated value between these two

		ADCControl1_MicAmpL_0000 = 0b00000000, // Left channel PGA gain, 0dB (default), Floor((Expected dB between [0dB, 24dB]) / 3dB) << 4 would be the interpolated value between these
		ADCControl1_MicAmpL_1000 = 0b10000000, // Left channel PGA gain, 24dB, Floor((Expected dB between [0dB, 24dB]) / 3dB) << 4 would be the interpolated value between these two

		ADCControl2_DSR_0 = 0b00000000, // LINPUT1-RINPUT1 (default)
		ADCControl2_DSR_1 = 0b00000100, // LINPUT2-RINPUT2

		ADCControl2_DSSEL_0 = 0b00000000, // use one DS Reg11[7] (default)
		ADCControl2_DSSEL_1 = 0b00001000, // DSL=Reg11[7], DSR=Reg10[2]

		ADCControl2_RINSEL_00 = 0b00000000, // Right channel input select RINPUT1 (default)
		ADCControl2_RINSEL_01 = 0b00010000, // Right channel input select RINPUT2
		ADCControl2_RINSEL_10 = 0b00100000, // Right channel input select reserved
		ADCControl2_RINSEL_11 = 0b00110000, // Right channel input select L-R differential (either LINPUT1-RINPUT1 or LINPUT2-RINPUT2, selected by DS)

		ADCControl2_LINSEL_00 = 0b00000000, // Left channel input select LINPUT1 (default)
		ADCControl2_LINSEL_01 = 0b01000000, // Left channel input select LINPUT2
		ADCControl2_LINSEL_10 = 0b10000000, // Left channel input select reserved
		ADCControl2_LINSEL_11 = 0b11000000, // Left channel input select L-R differential (either LINPUT1-RINPUT1 or LINPUT2-RINPUT2, selected by DS)

		ADCControl3_TRI_0 = 0b00000000, // ASDOUT is ADC normal output (default)
		ADCControl3_TRI_1 = 0b00000100, // ASDOUT tri-stated, ALRCK, DLRCK and SCLK are inputs

		ADCControl3_MONOMIX_00 = 0b00000000, // stereo (default)
		ADCControl3_MONOMIX_01 = 0b00001000, // analog mono mix to left ADC
		ADCControl3_MONOMIX_10 = 0b00010000, // analog mono mix to right ADC
		ADCControl3_MONOMIX_11 = 0b00011000, // reserved

		ADCControl3_DS_0 = 0b00000000, // Differential input select LINPUT1-RINPUT1 (default)
		ADCControl3_DS_1 = 0b10000000, // Differential input select LINPUT2-RINPUT2

		ADCControl4_ADCFORMAT_00 = 0b00000000, // I2S serial audio data format
		ADCControl4_ADCFORMAT_01 = 0b00000001, // left justify serial audio data format
		ADCControl4_ADCFORMAT_10 = 0b00000010, // right justify serial audio data format
		ADCControl4_ADCFORMAT_11 = 0b00000011, // DSP / PCM mode serial audio data format

		ADCControl4_ADCWL_000 = 0b00000000, // 24-bit serial audio data word length
		ADCControl4_ADCWL_001 = 0b00000100, // 20-bit serial audio data word length
		ADCControl4_ADCWL_010 = 0b00001000, // 18-bit serial audio data word length
		ADCControl4_ADCWL_011 = 0b00001100, // 16-bit serial audio data word length
		ADCControl4_ADCWL_100 = 0b00010000, // 32-bit serial audio data word length

		ADCControl4_ADCLRP_0 = 0b00000000, // I2S, left justified or right justified mode: left and right normal polarity, DSP/PCM mode: MSB is available on 2nd BCLK rising edge after ALRCK rising edge
		ADCControl4_ADCLRP_1 = 0b00100001, // I2S, left justified or right justified mode: left and right inverted polarity, DSP/PCM mode: MSB is available on 1st BCLK rising edge after ALRCK rising edge

		ADCControl4_DATSEL_00 = 0b00000000, // left data = left ADC, right data = right ADC
		ADCControl4_DATSEL_01 = 0b01000000, // left data = left ADC, right data = left ADC
		ADCControl4_DATSEL_10 = 0b10000000, // left data = right ADC, right data = right ADC
		ADCControl4_DATSEL_11 = 0b11000000, // left data = right ADC, right data = left ADC

		ADCControl5_ADCFsRatio_00000 = 0b00000000, // Master mode ADC MCLK to sampling frequency ratio 128
		ADCControl5_ADCFsRatio_00001 = 0b00000001, // Master mode ADC MCLK to sampling frequency ratio 192
		ADCControl5_ADCFsRatio_00010 = 0b00000010, // Master mode ADC MCLK to sampling frequency ratio 256
		ADCControl5_ADCFsRatio_00011 = 0b00000011, // Master mode ADC MCLK to sampling frequency ratio 384
		ADCControl5_ADCFsRatio_00100 = 0b00000100, // Master mode ADC MCLK to sampling frequency ratio 512
		ADCControl5_ADCFsRatio_00101 = 0b00000101, // Master mode ADC MCLK to sampling frequency ratio 576
		ADCControl5_ADCFsRatio_00110 = 0b00000110, // Master mode ADC MCLK to sampling frequency ratio 768 (default)
		ADCControl5_ADCFsRatio_00111 = 0b00000111, // Master mode ADC MCLK to sampling frequency ratio 1024
		ADCControl5_ADCFsRatio_01000 = 0b00001000, // Master mode ADC MCLK to sampling frequency ratio 1152
		ADCControl5_ADCFsRatio_01001 = 0b00001001, // Master mode ADC MCLK to sampling frequency ratio 1408
		ADCControl5_ADCFsRatio_01010 = 0b00001010, // Master mode ADC MCLK to sampling frequency ratio 1536
		ADCControl5_ADCFsRatio_01011 = 0b00001011, // Master mode ADC MCLK to sampling frequency ratio 2112
		ADCControl5_ADCFsRatio_01100 = 0b00001100, // Master mode ADC MCLK to sampling frequency ratio 2304
		ADCControl5_ADCFsRatio_10000 = 0b00010000, // Master mode ADC MCLK to sampling frequency ratio 125
		ADCControl5_ADCFsRatio_10001 = 0b00010001, // Master mode ADC MCLK to sampling frequency ratio 136
		ADCControl5_ADCFsRatio_10010 = 0b00010010, // Master mode ADC MCLK to sampling frequency ratio 250
		ADCControl5_ADCFsRatio_10011 = 0b00010011, // Master mode ADC MCLK to sampling frequency ratio 272
		ADCControl5_ADCFsRatio_10100 = 0b00010100, // Master mode ADC MCLK to sampling frequency ratio 375
		ADCControl5_ADCFsRatio_10101 = 0b00010101, // Master mode ADC MCLK to sampling frequency ratio 500
		ADCControl5_ADCFsRatio_10110 = 0b00010110, // Master mode ADC MCLK to sampling frequency ratio 544
		ADCControl5_ADCFsRatio_10111 = 0b00010111, // Master mode ADC MCLK to sampling frequency ratio 750
		ADCControl5_ADCFsRatio_11000 = 0b00011000, // Master mode ADC MCLK to sampling frequency ratio 1000
		ADCControl5_ADCFsRatio_11001 = 0b00011001, // Master mode ADC MCLK to sampling frequency ratio 1088
		ADCControl5_ADCFsRatio_11010 = 0b00011010, // Master mode ADC MCLK to sampling frequency ratio 1496
		ADCControl5_ADCFsRatio_11011 = 0b00011011, // Master mode ADC MCLK to sampling frequency ratio 1500
		ADCControl5_ADCFsRatio_Other = 0b00011111, // Master mode ADC MCLK to sampling frequency ratio reserved

		ADCControl5_ADCFsMode_0 = 0b00000000, // single speed mode (default)
		ADCControl5_ADCFsMode_1 = 0b00100000, // double speed mode

		ADCControl6_ADC_HPF_R_0 = 0b00000000, // disable ADC right channel high pass filter
		ADCControl6_ADC_HPF_R_1 = 0b00010000, // enable ADC right channel high pass filter (default)

		ADCControl6_ADC_HPF_L_0 = 0b00000000, // disable ADC left channel high pass filter
		ADCControl6_ADC_HPF_L_1 = 0b00100000, // enable ADC left channel high pass filter (default)

		ADCControl6_ADC_invR_0 = 0b00000000, // normal (default)
		ADCControl6_ADC_invR_1 = 0b01000000, // right channel polarity inverted

		ADCControl6_ADC_invL_0 = 0b00000000, // normal (default)
		ADCControl6_ADC_invL_1 = 0b10000001, // left channel polarity inverted

		ADCControl7_ADCMute_0 = 0b00000000, // normal (default)
		ADCControl7_ADCMute_1 = 0b00000100, // mute ADC digital output

		ADCControl7_ADCLeR_0 = 0b00000000, // normal (default
		ADCControl7_ADCLeR_1 = 0b00001000, // both channel gain control is set by ADC left gain control register

		ADCControl7_ADCSoftRamp_0 = 0b00000000, // disabled digital volume control soft ramp
		ADCControl7_ADCSoftRamp_1 = 0b00010000, // enabled digital volume control soft ramp (default)

		ADCControl7_ADCRampRate_00 = 0b00000000, // 0.5dB per 4 LRCK digital volume control ramp rate (default)
		ADCControl7_ADCRampRate_01 = 0b01000000, // 0.5dB per 8 LRCK digital volume control ramp rate
		ADCControl7_ADCRampRate_10 = 0b10000000, // 0.5dB per 16 LRCK digital volume control ramp rate
		ADCControl7_ADCRampRate_11 = 0b11000000, // 0.5dB per 32 LRCK digital volume control ramp rate

		ADCControl8_LADCVOL_00000000 = 0b00000000, // Digital volume control attenuates the signal in 0.5dB incremental from 0dB to –96dB. 0dB, Floor((Expected dB between [-96dB, 0dB]) * -2) would be the interpolated value between these two
		ADCControl8_LADCVOL_11000000 = 0b11000000, // Digital volume control attenuates the signal in 0.5dB incremental from 0dB to –96dB. -96dB (default), Floor((Expected dB between [-96dB, 0dB]) * -2) would be the interpolated value between these two

		ADCControl9_RADCVOL_00000000 = 0b00000000, // Digital volume control attenuates the signal in 0.5dB incremental from 0dB to –96dB. 0dB, Floor((Expected dB between [-96dB, 0dB]) * -2) would be the interpolated value between these two
		ADCControl9_RADCVOL_11000000 = 0b11000000, // Digital volume control attenuates the signal in 0.5dB incremental from 0dB to –96dB. -96dB, Floor((Expected dB between [-96dB, 0dB]) * -2) would be the interpolated value between these two

		ADCControl10_MINGAIN_000 = 0b00000000, // Set minimum gain of PGA, -12dB, Floor(((Expected dB between [-12dB, 30dB]) + 12dB) / 6dB) would be the interpolated value between these two
		ADCControl10_MINGAIN_111 = 0b00000111, // Set minimum gain of PGA, 30dB, Floor(((Expected dB between [-12dB, 30dB]) + 12dB) / 6dB) would be the interpolated value between these two

		ADCControl10_MAXGAIN_000 = 0b00000000, // Set maximum gain of PGA -6.5dB, Floor(((Expected dB between [-6.5dB, 35.5dB]) + 6.5dB) / 6dB) << 3 would be the interpolated value between these two
		ADCControl10_MAXGAIN_111 = 0b00111000, // Set maximum gain of PGA 35.5dB, Floor(((Expected dB between [-6.5dB, 35.5dB]) + 6.5dB) / 6dB) << 3 would be the interpolated value between these two

		ADCControl10_ALCSEL_00 = 0b00000000, // ALC off
		ADCControl10_ALCSEL_01 = 0b01000000, // ALC right channel only
		ADCControl10_ALCSEL_10 = 0b10000000, // ALC left channel only
		ADCControl10_ALCSEL_11 = 0b11000000, // ALC stereo

		ADCControl11_ALCHLD_0000 = 0b00000000, // ALC hold time before gain is increased, 0ms, Floor(Log2((Expected time between [0ms, 1360ms] + 1000ms) / 0.67ms) - 1000ms) would be the interpolated value between these two
		ADCControl11_ALCHLD_1010 = 0b00001010, // ALC hold time before gain is increased, 1360ms, Floor(Log2((Expected time between [0ms, 1360ms] + 1000ms) / 0.67ms) - 1000ms) would be the interpolated value between these two

		ADCControl11_ALCLVL_0000 = 0b00000000, // ALC target, -16.5dB, Floor(((Expected dB between [-16.5dB, -1.5dB]) + 16.5dB) / 1.5dB) << 4 would be the interpolated value between these two
		ADCControl11_ALCLVL_1010 = 0b10100000, // ALC target, -1.5dB, Floor(((Expected dB between [-16.5dB, -1.5dB]) + 16.5dB) / 1.5dB) << 4 would be the interpolated value between these two

		ADCControl12_ALCATK_0000 = 0b00000000, // ALC attack (gain ramp down) time, ALC mode/limiter mode, 0.104ms/0.0227ms, Floor(Log2(Expected time between [0.104ms, 106ms] / 0.053ms)) would be the interpolated value between these two
		ADCControl12_ALCATK_1010 = 0b00001010, // ALC attack (gain ramp down) time, ALC mode/limiter mode, 106ms/23.2ms, Floor(Log2(Expected time between [0.104ms, 106ms] / 0.053ms)) would be the interpolated value between these two

		ADCControl12_ALCDCY_0000 = 0b00000000, // ALC decay (gain ramp up) time, ALC mode/limiter mode, 0.410ms/0.0908ms, Floor(Log2(Expected time between [0.410ms, 420ms] / 0.21ms)) << 4 would be the interpolated value between these two
		ADCControl12_ALCDCY_1010 = 0b10100000, // ALC decay (gain ramp up) time, ALC mode/limiter mode, 420ms/93ms, Floor(Log2(Expected time between [0.410ms, 420ms] / 0.21ms)) << 4 would be the interpolated value between these two

		ADCControl13_WIN_SIZE_00110 = 0b00000110, // Windows size for peak detector，set the window size to N*16 samples, 96, Floor((Expected number between [96, 496]) / 16) would be the interpolated value between these two
		ADCControl13_WIN_SIZE_11111 = 0b00011111, // Windows size for peak detector，set the window size to N*16 samples, 496, Floor((Expected number between [96, 496]) / 16) would be the interpolated value between these two

		ADCControl13_TIME_OUT_0 = 0b00000000, // Zero Cross time out, disable (default)
		ADCControl13_TIME_OUT_1 = 0b00100000, // Zero Cross time out, enable

		ADCControl13_ALCZC_0 = 0b00000000, // ALC uses zero cross detection circuit, disable (recommended)
		ADCControl13_ALCZC_1 = 0b01000000, // ALC uses zero cross detection circuit, enable

		ADCControl13_ALCMODE_0 = 0b00000000, // Determines the ALC mode of operation, ALC mode (Normal Operation)
		ADCControl13_ALCMODE_1 = 0b10000000, // Determines the ALC mode of operation, Limiter mode.

		ADCControl14_NGAT_0 = 0b00000000, // Noise gate function enable, disable
		ADCControl14_NGAT_1 = 0b00000001, // Noise gate function enable, enable

		ADCControl14_NGG_00 = 0b00000000, // Noise gate type, PGA gain held constant
		ADCControl14_NGG_01 = 0b00000010, // Noise gate type, mute ADC output
		ADCControl14_NGG_11 = 0b00000110, // Noise gate type, reserved

		ADCControl14_NGTH_00000 = 0b00000000, // Noise gate threshold, -76.5dBFS, Floor(((Expected dBFS between [-76.5dBFS, -30dBFS]) + 76.5dBFS) / 1.5dBFS) << 3 would be the interpolated value between these two
		ADCControl14_NGTH_11111 = 0b11111000, // Noise gate threshold, -30dBFS, Floor(((Expected dBFS between [-76.5dBFS, -30dBFS]) + 76.5dBFS) / 1.5dBFS) << 3 would be the interpolated value between these two

		DACControl1_DACFORMAT_00 = 0b00000000, // I2S serial audio data format
		DACControl1_DACFORMAT_01 = 0b00000010, // left justify serial audio data format
		DACControl1_DACFORMAT_10 = 0b00000100, // right justify serial audio data format
		DACControl1_DACFORMAT_11 = 0b00000110, // DSP/PCM mode serial audio data format

		DACControl1_DACWL_000 = 0b00000000, // 24-bit serial audio data word length
		DACControl1_DACWL_001 = 0b00001000, // 20-bit serial audio data word length
		DACControl1_DACWL_010 = 0b00010000, // 18-bit serial audio data word length
		DACControl1_DACWL_011 = 0b00011000, // 16-bit serial audio data word length
		DACControl1_DACWL_100 = 0b00100000, // 32-bit serial audio data word length

		DACControl1_DACLRP_0 = 0b00000000, // I2S, left justified or right justified mode, left and right normal polarity, MSB is available on 2nd BCLK rising edge after ALRCK rising edge
		DACControl1_DACLRP_1 = 0b01000000, // I2S, left justified or right justified mode, left and right inverted polarity, MSB is available on 1st BCLK rising edge after ALRCK rising edgeLRCK Polarity

		DACControl1_DACLRSWAP_0 = 0b00000000, // normal
		DACControl1_DACLRSWAP_1 = 0b10000000, // left and right channel data swap

		DACCOntrol2_DACFsRatio_00000 = 0b00000000, // Master mode DAC MCLK to sampling frequency ratio, 128;
		DACCOntrol2_DACFsRatio_00001 = 0b00000001, // Master mode DAC MCLK to sampling frequency ratio, 192;
		DACCOntrol2_DACFsRatio_00010 = 0b00000010, // Master mode DAC MCLK to sampling frequency ratio, 256;
		DACCOntrol2_DACFsRatio_00011 = 0b00000011, // Master mode DAC MCLK to sampling frequency ratio, 384;
		DACCOntrol2_DACFsRatio_00100 = 0b00000100, // Master mode DAC MCLK to sampling frequency ratio, 512;
		DACCOntrol2_DACFsRatio_00101 = 0b00000101, // Master mode DAC MCLK to sampling frequency ratio, 576;
		DACCOntrol2_DACFsRatio_00110 = 0b00000110, // Master mode DAC MCLK to sampling frequency ratio, 768; (default)
		DACCOntrol2_DACFsRatio_00111 = 0b00000111, // Master mode DAC MCLK to sampling frequency ratio, 1024;
		DACCOntrol2_DACFsRatio_01000 = 0b00001000, // Master mode DAC MCLK to sampling frequency ratio, 1152;
		DACCOntrol2_DACFsRatio_01001 = 0b00001001, // Master mode DAC MCLK to sampling frequency ratio, 1408;
		DACCOntrol2_DACFsRatio_01010 = 0b00001010, // Master mode DAC MCLK to sampling frequency ratio, 1536;
		DACCOntrol2_DACFsRatio_01011 = 0b00001011, // Master mode DAC MCLK to sampling frequency ratio, 2112;
		DACCOntrol2_DACFsRatio_01100 = 0b00001100, // Master mode DAC MCLK to sampling frequency ratio, 2304;
		DACCOntrol2_DACFsRatio_10000 = 0b00010000, // Master mode DAC MCLK to sampling frequency ratio, 125;
		DACCOntrol2_DACFsRatio_10001 = 0b00010001, // Master mode DAC MCLK to sampling frequency ratio, 136;
		DACCOntrol2_DACFsRatio_10010 = 0b00010010, // Master mode DAC MCLK to sampling frequency ratio, 250;
		DACCOntrol2_DACFsRatio_10011 = 0b00010011, // Master mode DAC MCLK to sampling frequency ratio, 272;
		DACCOntrol2_DACFsRatio_10100 = 0b00010100, // Master mode DAC MCLK to sampling frequency ratio, 375;
		DACCOntrol2_DACFsRatio_10101 = 0b00010101, // Master mode DAC MCLK to sampling frequency ratio, 500;
		DACCOntrol2_DACFsRatio_10110 = 0b00010110, // Master mode DAC MCLK to sampling frequency ratio, 544;
		DACCOntrol2_DACFsRatio_10111 = 0b00010111, // Master mode DAC MCLK to sampling frequency ratio, 750;
		DACCOntrol2_DACFsRatio_11000 = 0b00011000, // Master mode DAC MCLK to sampling frequency ratio, 1000;
		DACCOntrol2_DACFsRatio_11001 = 0b00011001, // Master mode DAC MCLK to sampling frequency ratio, 1088;
		DACCOntrol2_DACFsRatio_11010 = 0b00011010, // Master mode DAC MCLK to sampling frequency ratio, 1496;
		DACCOntrol2_DACFsRatio_11011 = 0b00011011, // Master mode DAC MCLK to sampling frequency ratio, 1500;

		DACCOntrol2_DACFsMode_0 = 0b00000000, // single speed mode (default
		DACCOntrol2_DACFsMode_1 = 0b00100000, // double speed mode

		DACControl3_DACMute_0 = 0b00000000, // normal (default)
		DACControl3_DACMute_1 = 0b00000100, // mute analog outputs for both channels

		DACControl3_DACLeR_0 = 0b00000000, // normal (default)
		DACControl3_DACLeR_1 = 0b00001000, // both channel gain control is set by DAC left gain control register

		DACControl3_DACSoftRamp_0 = 0b00000000, // disabled digital volume control soft ramp
		DACControl3_DACSoftRamp_1 = 0b00100000, // enabled digital volume control soft ramp (default)

		DACControl3_DACRampRate_00 = 0b00000000, // 0.5dB per 4 LRCK digital volume control ramp rate (default)
		DACControl3_DACRampRate_01 = 0b01000000, // 0.5dB per 32 LRCK digital volume control ramp rate
		DACControl3_DACRampRate_10 = 0b10000000, // 0.5dB per 64 LRCK digital volume control ramp rate
		DACControl3_DACRampRate_11 = 0b11000001, // 0.5dB per 128 LRCK digital volume control ramp rate

		DACControl4_LDACVOL_00000000 = 0b00000000, // Digital volume control attenuates the signal in 0.5dB incremental from 0dB to –96dB, 0dB, Floor((Expected dB between [-96dB, 0dB]) * -2) would be the interpolated value between these two
		DACControl4_LDACVOL_11000000 = 0b11000000, // Digital volume control attenuates the signal in 0.5dB incremental from 0dB to –96dB, –96dB, Floor((Expected dB between [-96dB, 0dB]) * -2) would be the interpolated value between these two

		DACControl5_RDACVOL_00000000 = 0b00000000, // Digital volume control attenuates the signal in 0.5dB incremental from 0dB to –96dB, 0dB, Floor((Expected dB between [-96dB, 0dB]) * -2) would be the interpolated value between these two
		DACControl5_RDACVOL_11000000 = 0b00000001, // Digital volume control attenuates the signal in 0.5dB incremental from 0dB to –96dB, –96dB, Floor((Expected dB between [-96dB, 0dB]) * -2) would be the interpolated value between these two

		DACControl6_DEEMP_0 = 0b00000000, // disable digital click free power up and down
		DACControl6_DEEMP_1 = 0b00001000, // enable digital click free power up and down (default)

		DACControl6_DAC_invR_0 = 0b00000000, // normal DAC right channel analog output no phase inversion (default)
		DACControl6_DAC_invR_1 = 0b00010000, // normal DAC right analog output 180 degree phase inversion

		DACControl6_DAC_invL_0 = 0b00000000, // normal DAC left channel analog output no phase inversion (default)
		DACControl6_DAC_invL_1 = 0b00100000, // normal DAC left channel analog output 180 degree phase inversion

		DACControl6_dacVref_PDN_00 = 0b00000000, // de-emphasis frequency disabled (default)
		DACControl6_dacVref_PDN_01 = 0b01000000, // 32 KHz de-emphasis frequency in single speed mode
		DACControl6_dacVref_PDN_10 = 0b10000000, // 44.1 KHz de-emphasis frequency in single speed mode
		DACControl6_dacVref_PDN_11 = 0b11000000, // 48 KHz de-emphasis frequency in single speed mode

		DACControl7_Vpp_scale_00 = 0b00000000, // Vpp set at 3.5V (0.7 modulation index) (default)
		DACControl7_Vpp_scale_01 = 0b00000001, // Vpp set at 4.0V
		DACControl7_Vpp_scale_10 = 0b00000010, // Vpp set at 3.0V
		DACControl7_Vpp_scale_11 = 0b00000011, // Vpp set at 2.5V

		DACControl7_SE_000 = 0b00000000, // SE strength, 0, (Expected length between [0, 7]) << 2 would be the interpolated value between these two
		DACControl7_SE_111 = 0b00011100, // SE strength, 7, (Expected length between [0, 7]) << 2 would be the interpolated value between these two

		DACControl7_Mono_0 = 0b00000000, // stereo (default)
		DACControl7_Mono_1 = 0b00100000, // mono (L+R)/2 into DACL and DACR

		DACControl7_ZeroR_0 = 0b00000000, // normal (default)
		DACControl7_ZeroR_1 = 0b01000000, // set Right Channel DAC output all zero

		DACControl7_ZeroL_0 = 0b00000000, // normal (default)
		DACControl7_ZeroL_1 = 0b10000000, // set Left Channel DAC output all zero

		DACControl8_Shelving_a_24_29_000000 = 0b00000000, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}
		DACControl8_Shelving_a_24_29_111111 = 0b00111111, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}

		DACControl9_Shelving_a_16_23_00000000 = 0b00000000, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}
		DACControl9_Shelving_a_16_23_11111111 = 0b11111111, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}

		DACControl10_Shelving_a_8_15_00000000 = 0b00000000, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}
		DACControl10_Shelving_a_8_15_11111111 = 0b11111111, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}

		DACControl11_Shelving_a_0_7_00000000 = 0b00000000, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}
		DACControl11_Shelving_a_0_7_11111111 = 0b11111111, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}

		DACControl12_Shelving_b_24_29_000000 = 0b00000000, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}
		DACControl12_Shelving_b_24_29_111111 = 0b00111111, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}

		DACControl13_Shelving_b_16_23_00000000 = 0b00000000, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}
		DACControl13_Shelving_b_16_23_11111111 = 0b11111111, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}

		DACControl14_Shelving_b_8_15_00000000 = 0b00000000, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}
		DACControl14_Shelving_b_8_15_11111111 = 0b11111111, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}

		DACControl15_Shelving_b_0_7_00000000 = 0b00000000, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}
		DACControl15_Shelving_b_0_7_11111111 = 0b11111111, // 30-bit a coefficient for shelving filter, Default value is {5'h0f, 5'h1f, 5'h0f, 5'h1f, 5'h0f, 5'h1f}

		DACControl16_RMIXSEL_000 = 0b00000000, // Right input select for output mix, RIN1 (default)
		DACControl16_RMIXSEL_001 = 0b00000001, // Right input select for output mix, RIN2
		DACControl16_RMIXSEL_010 = 0b00000010, // Right input select for output mix, reserved
		DACControl16_RMIXSEL_011 = 0b00000011, // Right input select for output mix, right ADC P input (after mic amplifier)
		DACControl16_RMIXSEL_100 = 0b00000100, // Right input select for output mix, right ADC N input (after mic amplifier)

		DACControl16_LMIXSEL_000 = 0b00000000, // Left input select for output mix, LIN1 (default)
		DACControl16_LMIXSEL_001 = 0b00001000, // Left input select for output mix, LIN2
		DACControl16_LMIXSEL_010 = 0b00010000, // Left input select for output mix, reserved
		DACControl16_LMIXSEL_011 = 0b00011000, // Left input select for output mix, left ADC P input (after mic amplifier)
		DACControl16_LMIXSEL_100 = 0b00100000, // Left input select for output mix, left ADC N input (after mic amplifier)

		DACControl17_LI2LOVOL_000 = 0b00000000, // LIN signal to left mixer gain, 6dB, (7 - Floor(((Expected dB between [-15dB, 6dB]) + 15dB) / 3dB)) << 3 would be the interpolated value between these two
		DACControl17_LI2LOVOL_111 = 0b00111000, // LIN signal to left mixer gain, -15dB (default), (7 - Floor(((Expected dB between [-15dB, 6dB]) + 15dB) / 3dB)) << 3 would be the interpolated value between these two

		DACControl17_LI2LO_0 = 0b00000000, // LIN signal to left mixer disable (default)
		DACControl17_LI2LO_1 = 0b01000000, // LIN signal to left mixer enable

		DACControl17_LD2LO_0 = 0b00000000, // left DAC to left mixer disable (default)
		DACControl17_LD2LO_1 = 0b10000000, // left DAC to left mixer enable

		DACControl20_RI2ROVOL_000 = 0b00000000, // RIN signal to right mixer gain, 6dB, (7 - Floor(((Expected dB between [-15dB, 6dB]) + 15dB) / 3dB)) << 3 would be the interpolated value between these two
		DACControl20_RI2ROVOL_111 = 0b00111000, // RIN signal to right mixer gain, -15dB (default), (7 - Floor(((Expected dB between [-15dB, 6dB]) + 15dB) / 3dB)) << 3 would be the interpolated value between these two

		DACControl20_RI2RO_0 = 0b00000000, // RIN signal to right mixer disable (default)
		DACControl20_RI2RO_1 = 0b01000000, // RIN signal to right mixer enable

		DACControl20_RD2RO_0 = 0b00000000, // right DAC to right mixer disable (default)
		DACControl20_RD2RO_1 = 0b10000000, // right DAC to right mixer enable

		DACControl21_dac_dll_pwd_0 = 0b00000000, // normal (default)
		DACControl21_dac_dll_pwd_1 = 0b00000100, // DAC DLL power down

		DACControl21_adc_dll_pwd_0 = 0b00000000, // normal (default)
		DACControl21_adc_dll_pwd_1 = 0b00001000, // ADC DLL power down

		DACControl21_mclk_dis_0 = 0b00000000, // normal (default)
		DACControl21_mclk_dis_1 = 0b00010000, // disable MCLK input from PAD

		DACControl21_offset_dis_0 = 0b00000000, // disable offset (default)
		DACControl21_offset_dis_1 = 0b00100000, // enable offset

		DACControl21_lrck_sel_0 = 0b00000000, // Master mode, if slrck = 1 then, use DAC LRCK (default)
		DACControl21_lrck_sel_1 = 0b01000000, // Master mode, if slrck = 1 then, use ADC LRCK

		DACControl21_slrck_0 = 0b00000000, // DACLRC and ADCLRC separate (default)
		DACControl21_slrck_1 = 0b10000000, // DACLRC and ADCLRC same

		DACControl22_offset_00000000 = 0b00000000, // DC offset
		DACControl22_offset_11111111 = 0b11111111, // DC offset

		DACControl23_VROI_0 = 0b00000000, // 1.5k VREF to analog output resistance (default)
		DACControl23_VROI_1 = 0b00010000, // 40k VREF to analog output resistance

		DACControl24_LOUT1VOL_000000 = 0b00000000, // LOUT1 volume, -45dB, Floor(((Expected dB between [-45dB, 4.5dB]) + 45dB) / 1.5dB) would be the interpolated value between these two
		DACControl24_LOUT1VOL_100001 = 0b00100001, // LOUT1 volume, 4.5dB, Floor(((Expected dB between [-45dB, 4.5dB]) + 45dB) / 1.5dB) would be the interpolated value between these two

		DACControl25_ROUT1VOL_000000 = 0b00000000, // ROUT1 volume, -45dB, Floor(((Expected dB between [-45dB, 4.5dB]) + 45dB) / 1.5dB) would be the interpolated value between these two
		DACControl25_ROUT1VOL_100001 = 0b00100001, // ROUT1 volume, 4.5dB, Floor(((Expected dB between [-45dB, 4.5dB]) + 45dB) / 1.5dB) would be the interpolated value between these two

		DACControl26_LOUT2VOL_000000 = 0b00000000, // LOUT2 volume, -45dB, Floor(((Expected dB between [-45dB, 4.5dB]) + 45dB) / 1.5dB) would be the interpolated value between these two
		DACControl26_LOUT2VOL_100001 = 0b00100001, // LOUT2 volume, 4.5dB, Floor(((Expected dB between [-45dB, 4.5dB]) + 45dB) / 1.5dB) would be the interpolated value between these two

		DACControl27_ROUT2VOL_000000 = 0b00000000, // ROUT2 volume, -45dB, Floor(((Expected dB between [-45dB, 4.5dB]) + 45dB) / 1.5dB) would be the interpolated value between these two
		DACControl27_ROUT2VOL_100001 = 0b00100001, // ROUT2 volume, 4.5dB, Floor(((Expected dB between [-45dB, 4.5dB]) + 45dB) / 1.5dB) would be the interpolated value between these two

		DACControl29_hpLout1_ref2_0 = 0b00000000, // Reserved

		DACControl29_hpLout1_ref1_0 = 0b00000000, // Reserved

		DACControl30_MREF2_0 = 0b00000000, // Reserved

		DACControl30_MREF1_0 = 0b00000000, // Reserved

		DACControl30_mixer_ref2_0 = 0b00000000, // Reserved

		DACControl30_mixer_ref1_0 = 0b00000000, // Reserved

		DACControl30_spkLout2_ref2_0 = 0b00000000, // Reserved

		DACControl30_spkLout2_ref1_0 = 0b00000000, // Reserved
	};

	static uint8 Read(Registers Register)
	{
		uint8 value = I2CUtils::Read(ADDRESS, (uint8)Register);

		Log::WriteDebug(TAG, "Reading from address: 0x%02x register: 0x%02x the value: 0x%02x", ADDRESS, Register, value);

		return value;
	}

	static void Write(Registers Register, uint8 Value)
	{
		Log::WriteDebug(TAG, "Writing to address: 0x%02x register: 0x%02x the value: 0x%02x", ADDRESS, Register, Value);

		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)Register, Value));
	}

	static Values Read(Registers Register, Masks Mask)
	{
		uint8 value = I2CUtils::Read(ADDRESS, (uint8)Register) & (uint8)Mask;

		Log::WriteDebug(TAG, "Reading from address: 0x%02x register: 0x%02x the value: 0x%02x with the mask: 0x%02x", ADDRESS, Register, value, Mask);

		return (Values)value;
	}

	static void Write(Registers Register, Values Value, Masks Mask)
	{
		ASSERT(((uint8)Value & ~(uint8)Mask) == 0, TAG, "Mask %i and Value %i are not compatible", (uint8)Mask, (uint8)Value);

		Log::WriteDebug(TAG, "Writing to address: 0x%02x register: 0x%02x the value: 0x%02x with the mask: 0x%02x", ADDRESS, Register, Value, Mask);

		uint8 otherBitValues = I2CUtils::Read(ADDRESS, (uint8)Register) & ~(uint8)Mask;

		CHECK_CALL(I2CUtils::Write(ADDRESS, (uint8)Register, otherBitValues | (uint8)Value));
	}

public:
	static const int32 ADDRESS;

private:
	static const char *TAG;
};

const int32 ES8388Control::ADDRESS = 0x10;

const char *ES8388Control::TAG = "ES83288Control";

#endif