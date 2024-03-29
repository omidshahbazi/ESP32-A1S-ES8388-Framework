#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <inttypes.h>
#include <hal/gpio_types.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

#define KHz 1000
#define MHz 1000 * KHz

#define SAMPLE_RATE_8000 8 * KHz
#define SAMPLE_RATE_15750 15.75 * KHz
#define SAMPLE_RATE_16000 16 * KHz
#define SAMPLE_RATE_22050 22.05 * KHz
#define SAMPLE_RATE_24000 24 * KHz
#define SAMPLE_RATE_32000 32 * KHz
#define SAMPLE_RATE_44100 44.1 * KHz
#define SAMPLE_RATE_48000 48 * KHz
#define SAMPLE_RATE_96000 96 * KHz
#define SAMPLE_RATE_192000 192 * KHz
#define SAMPLE_RATE_320000 320 * KHz

#define MIN_SAMPLE_RATE SAMPLE_RATE_8000
#define MAX_SAMPLE_RATE SAMPLE_RATE_320000

#define MIN_FREQUENCY 20.0
#define MAX_FREQUENCY 20.0 * KHz

enum class GPIOPins
{
	Pin4 = (uint8)GPIO_NUM_4,	// Analog/Digital - Input/Output (Cannot be Connected or Must be Connected to Zero in Root Mode)
	Pin12 = (uint8)GPIO_NUM_12, // Analog/Digital - Input/Output (Cannot be Connected or Must be Connected to Zero in Root Mode and Start Up)
	Pin13 = (uint8)GPIO_NUM_13, // Analog/Digital - Input (Cannot be Connected or Must be Connected to Zero in Root Mode)
	Pin14 = (uint8)GPIO_NUM_14, // Analog/Digital - Input/Output
	Pin15 = (uint8)GPIO_NUM_15, // Analog/Digital - Input/Output
	Pin34 = (uint8)GPIO_NUM_34, // Analog/Digital - Input (Cannot be Connected or Must be Connected to Zero in Root Mode)

	Pin19 = (uint8)GPIO_NUM_19, // Digital - Input/Output
	Pin21 = (uint8)GPIO_NUM_21, // Digital - Input/Output
	Pin22 = (uint8)GPIO_NUM_22, // Digital - Input/Output (Cannot be Connected in Root Mode)
	Pin23 = (uint8)GPIO_NUM_23	// Digital - Input/Output
};

#endif