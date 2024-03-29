#pragma once

#include "DSP/Common.h"
#include <hal/gpio_types.h>

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