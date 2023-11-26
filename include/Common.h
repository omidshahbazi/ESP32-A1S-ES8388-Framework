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

#define SAMPLE_RATE_44100 44.1 * KHz
#define SAMPLE_RATE_48000 48 * KHz
#define SAMPLE_RATE_96000 96 * KHz
#define SAMPLE_RATE_192000 192 * KHz

#define MIN_SAMPLE_RATE SAMPLE_RATE_44100
#define MAX_SAMPLE_RATE SAMPLE_RATE_192000

#define MIN_FREQUENCY 1.0
#define MAX_FREQUENCY 20154.0

enum class GPIOPins
{
	Pin0 = (uint8)GPIO_NUM_0,	// Analog/Digital - Output
	Pin2 = (uint8)GPIO_NUM_2,	// Analog/Digital - Input/Output
	Pin4 = (uint8)GPIO_NUM_4,	// Analog/Digital - Input/Output
	Pin12 = (uint8)GPIO_NUM_12, // Analog/Digital - Input/Output
	Pin13 = (uint8)GPIO_NUM_13, // Analog/Digital - Input
	Pin14 = (uint8)GPIO_NUM_14, // Analog/Digital - Input/Output
	Pin15 = (uint8)GPIO_NUM_15, // Analog/Digital - Input/Output
	Pin34 = (uint8)GPIO_NUM_34, // Analog/Digital - Input

	Pin5 = (uint8)GPIO_NUM_5,	// Digital - Input/Output
	Pin18 = (uint8)GPIO_NUM_18, // Digital - Input/Output
	Pin19 = (uint8)GPIO_NUM_19, // Digital - Input/Output
	Pin21 = (uint8)GPIO_NUM_21, // Digital - Input/Output
	Pin22 = (uint8)GPIO_NUM_22, // Digital - Input/Output
	Pin23 = (uint8)GPIO_NUM_23	// Digital - Input/Output
};

#endif