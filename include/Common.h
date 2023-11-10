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

enum class GPIOPins
{
	Pin0 = (uint8)GPIO_NUM_0,
	Pin2 = (uint8)GPIO_NUM_2,
	Pin4 = (uint8)GPIO_NUM_4,
	Pin5 = (uint8)GPIO_NUM_5,
	Pin12 = (uint8)GPIO_NUM_12,
	Pin13 = (uint8)GPIO_NUM_13,
	Pin14 = (uint8)GPIO_NUM_14,
	Pin15 = (uint8)GPIO_NUM_15,
	Pin18 = (uint8)GPIO_NUM_18,
	Pin19 = (uint8)GPIO_NUM_19,
	Pin21 = (uint8)GPIO_NUM_21,
	Pin22 = (uint8)GPIO_NUM_22,
	Pin23 = (uint8)GPIO_NUM_23,
	Pin34 = (uint8)GPIO_NUM_34
};

#endif