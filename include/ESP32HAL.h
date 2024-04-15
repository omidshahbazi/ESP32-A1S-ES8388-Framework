#pragma once
#ifndef ESP32_HAL_H
#define ESP32_HAL_H

#include "framework/include/DSP/IHAL.h"
#include "Common.h"
#include "DSP/Debug.h"
#include <Esp.h>

const int PWM_FREQUENCY = 240;
const int PWM_CHANNEL = 1;

class ESP32HAL : public IHAL
{
private:
	struct PWMChannel
	{
	public:
		bool Used;
		uint8 Channel;
		uint8 Pin;
	};

public:
	ESP32HAL(void)
		: m_PWMResolution(0),
		  m_PWMMaxDutyCycle(0),
		  m_AnalogReadResolution(0),
		  m_MaxAnalogValue(0)
	{
		uint8 channelIndex = 0;
		for (auto &pwmChannel : m_PWMChannels)
			pwmChannel = {false, channelIndex++, 0};

		SetAnalogReadResolution(12);
		SetPWMResolution(16);
	}

private:
	void *Allocate(uint16 Size) override
	{
		uint32 ramType = MALLOC_CAP_DEFAULT;
		// if (FromExternalRAM)
		// 	ramType = MALLOC_CAP_SPIRAM;

		return heap_caps_malloc(Size, ramType);
	}

	void Deallocate(void *Memory) override
	{
		heap_caps_free(Memory);
	}

	bool IsAnAnaloglPin(uint8 Pin) const override
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

	bool IsADigitalPin(uint8 Pin) const override
	{
		return true;
	}

	bool IsAnInputPin(uint8 Pin) const override
	{
		return true;
	}

	bool IsAnOutputPin(uint8 Pin) const override
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

	bool IsAPWMPin(uint8 Pin) const override
	{
		switch (Pin)
		{
		case (uint8)GPIOPins::Pin4:
		case (uint8)GPIOPins::Pin12:
		case (uint8)GPIOPins::Pin14:
		case (uint8)GPIOPins::Pin15:
		case (uint8)GPIOPins::Pin19:
		case (uint8)GPIOPins::Pin22:
		case (uint8)GPIOPins::Pin23:
			return true;

		default:
			return false;
		}
	}

	void SetPWMResolution(uint8 Value) override
	{
		ASSERT(8 <= Value && Value <= 16, "Invalid Value");

		m_PWMResolution = Value;
		m_PWMMaxDutyCycle = (1 << m_PWMResolution) - 1;
	}

	uint8 GetPWMResolution(void) const override
	{
		return m_PWMResolution;
	}

	void SetPinMode(uint8 Pin, PinModes Mode) override
	{
		ASSERT(Mode != PinModes::Input || IsAnInputPin(Pin), "Pin %i is not an input pin", Pin);
		ASSERT(Mode != PinModes::Output || IsAnOutputPin(Pin), "Pin %i is not an output pin", Pin);
		ASSERT(Mode != PinModes::PWM || IsAPWMPin(Pin), "Pin %i is not an PWM pin", Pin);

		switch (Mode)
		{
		case PinModes::Input:
			pinMode(Pin, INPUT);
			break;

		case PinModes::Output:
			pinMode(Pin, OUTPUT);
			break;

		case PinModes::PWM:
			InitializePWM(Pin);
			break;
		}
	}

	void SetAnalogReadResolution(uint8 Value) override
	{
		ASSERT(8 <= Value && Value <= 12, "Invalid Value");

		m_AnalogReadResolution = Value;
		m_MaxAnalogValue = 1 << Value;

		analogReadResolution(m_AnalogReadResolution);
	}

	uint8 GetAnalogReadResolution(void) const override
	{
		return m_AnalogReadResolution;
	}

	float AnalogRead(uint8 Pin) const override
	{
		return (analogRead(Pin) / (float)m_MaxAnalogValue);
	}

	void AnalogWrite(uint8 Pin, float Value) override
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		analogWrite(Pin, Value * m_MaxAnalogValue);
	}

	bool DigitalRead(uint8 Pin) const override
	{
		return (digitalRead((uint8)Pin) != LOW);
	}

	void DigitalWrite(uint8 Pin, bool Value) override
	{
		digitalWrite(Pin, Value);
	}

	void PWMWrite(uint8 Pin, float Value) override
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		PWMChannel *channel = nullptr, *it = m_PWMChannels;
		for (auto &pwmChannel : m_PWMChannels)
		{
			if (!pwmChannel.Used || pwmChannel.Pin != Pin)
			{
				++it;

				continue;
			}

			channel = it;

			break;
		}

		ASSERT(channel != nullptr, "Couldn't find the channel attached to the Pin %i", Pin);

		ledcWrite(channel->Channel, Value * m_PWMMaxDutyCycle);
	}

	void Print(const char *Value) const
	{
		printf(Value);
	}

	void Break(void) const override
	{
		esp_restart();
	}

private:
	void InitializePWM(uint8 Pin)
	{
		PWMChannel *channel = nullptr, *it = m_PWMChannels;
		for (auto &pwmChannel : m_PWMChannels)
		{
			if (!pwmChannel.Used || pwmChannel.Pin != Pin)
			{
				++it;

				continue;
			}

			channel = it;

			break;
		}

		if (channel == nullptr)
		{
			it = m_PWMChannels;
			for (auto &pwmChannel : m_PWMChannels)
			{
				if (pwmChannel.Used)
				{
					++it;

					continue;
				}

				channel = it;

				break;
			}

			ASSERT(channel != nullptr, "Out of PWM channel");
		}

		channel->Pin = Pin;
		channel->Used = true;

		ledcSetup(channel->Channel, PWM_FREQUENCY, m_PWMResolution);

		ledcAttachPin(Pin, channel->Channel);
	}

private:
	PWMChannel m_PWMChannels[SOC_LEDC_CHANNEL_NUM];
	uint8 m_PWMResolution;
	uint32 m_PWMMaxDutyCycle;
	uint32 m_AnalogReadResolution;
	uint32 m_MaxAnalogValue;
};

#endif