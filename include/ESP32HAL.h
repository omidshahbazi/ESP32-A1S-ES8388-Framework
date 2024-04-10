#pragma once
#ifndef ESP32_HAL_H
#define ESP32_HAL_H

#include "framework/include/DSP/IHAL.h"
#include "Common.h"
#include <Esp.h>

class ESP32HAL : public IHAL
{
public:
	ESP32HAL(void)
		: m_MaxAnalogValue(0)
	{
		SetAnalogReadResolution(10);
	}

private:
	void *Allocate(uint16 Size)
	{
		uint32 ramType = MALLOC_CAP_DEFAULT;
		// if (FromExternalRAM)
		// 	ramType = MALLOC_CAP_SPIRAM;

		return heap_caps_malloc(Size, ramType);
	}

	void Deallocate(void *Memory)
	{
		heap_caps_free(Memory);
	}

	bool IsAnAnaloglPin(int8 Pin) const
	{
		switch (Pin)
		{
		case (uint8)GPIOPins::Pin4:
		case (uint8)GPIOPins::Pin12:
		case (uint8)GPIOPins::Pin13:
		case (uint8)GPIOPins::Pin14:
		case (uint8)GPIOPins::Pin15:
		case (uint8)GPIOPins::Pin34:
			return true;

		default:
			return false;
		}
	}

	bool IsADigitalPin(int8 Pin) const
	{
		return true;
	}

	bool IsAnInputPin(int8 Pin) const
	{
		return true;
	}

	bool IsAnOutputPin(int8 Pin) const
	{
		switch (Pin)
		{
		case (uint8)GPIOPins::Pin4:
		case (uint8)GPIOPins::Pin12:
		case (uint8)GPIOPins::Pin14:
		case (uint8)GPIOPins::Pin15:
		case (uint8)GPIOPins::Pin19:
		case (uint8)GPIOPins::Pin21:
		case (uint8)GPIOPins::Pin22:
		case (uint8)GPIOPins::Pin23:
			return true;

		default:
			return false;
		}
	}

	void SetPinMode(int8 Pin, PinModes Mode)
	{
		pinMode(Pin, (Mode == PinModes::Input ? INPUT : OUTPUT));
	}

	void SetAnalogReadResolution(int8 Bits)
	{
		analogReadResolution(Bits);

		m_MaxAnalogValue = 1 << Bits;
	}

	float AnalogRead(int8 Pin) const
	{
		return (analogRead(Pin) / (float)m_MaxAnalogValue);
	}

	void AnalogWrite(int8 Pin, float Value)
	{
		analogWrite(Pin, Value * m_MaxAnalogValue);
	}

	bool DigitalRead(int8 Pin) const
	{
		return (digitalRead((uint8)Pin) != LOW);
	}

	void DigitalWrite(int8 Pin, bool Value)
	{
		digitalWrite(Pin, Value);
	}

	void Print(const char *Value) const
	{
		printf(Value);
	}

	void Break(void) const
	{
		esp_restart();
	}

private:
	uint32 m_MaxAnalogValue;
};

#endif