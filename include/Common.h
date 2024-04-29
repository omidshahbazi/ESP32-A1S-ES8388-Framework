#pragma once

#include "DSP/Common.h"
#include <hal/gpio_types.h>

enum class GPIOPins
{
	Pin13 = (uint8)GPIO_NUM_13, // Analog/Digital - Input
	Pin14 = (uint8)GPIO_NUM_14, // Analog/Digital - Input/Output
	Pin15 = (uint8)GPIO_NUM_15, // Analog/Digital - Input/Output
	Pin34 = (uint8)GPIO_NUM_34, // Analog/Digital - Input

	Pin19 = (uint8)GPIO_NUM_19, // Digital - Input/Output
	Pin21 = (uint8)GPIO_NUM_21, // Digital - Input/Output
	Pin22 = (uint8)GPIO_NUM_22, // Digital - Input/Output
	Pin23 = (uint8)GPIO_NUM_23, // Digital - Input/Output

	COUNT = 34
};