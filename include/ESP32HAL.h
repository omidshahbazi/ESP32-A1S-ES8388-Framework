#pragma once
#ifndef ESP32_HAL_H
#define ESP32_HAL_H

#include "DSP/IHAL.h"
#include "Common.h"
#include "DSP/Debug.h"
#include <Esp.h>
#include <chrono>

using namespace std::chrono;

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

	void *Allocate(uint32 Size, bool OnSDRAM = false) override
	{
		uint32 ramType = MALLOC_CAP_DEFAULT;
		// if (OnSDRAM)
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

	float AnalogRead(uint8 Pin) const override
	{
		ASSERT(IsAnAnaloglPin(Pin), "Pin %i is not an analog pin", Pin);

		return (analogRead(Pin) / (float)m_MaxAnalogValue);
	}

	bool DigitalRead(uint8 Pin) const override
	{
		ASSERT(IsADigitalPin(Pin), "Pin %i is not an digital pin", Pin);

		return (digitalRead((uint8)Pin) != LOW);
	}

	void DigitalWrite(uint8 Pin, bool Value) override
	{
		ASSERT(IsADigitalPin(Pin), "Pin %i is not an digital pin", Pin);

		digitalWrite(Pin, Value);
	}

	void PWMWrite(uint8 Pin, float Value) override
	{
		ASSERT(0 <= Value && Value <= 1, "Invalid Value");

		PWMChannel *channel = FindPWMChannel(Pin);
		ASSERT(channel != nullptr, "Couldn't find the channel attached to the Pin %i", Pin);

		ledcWrite(channel->Channel, Value * m_PWMMaxDutyCycle);
	}

	float GetTimeSinceStartup(void) const override
	{
		return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count() / 1000.0F;
	}

	void Print(const char *Value) const
	{
		printf(Value);
	}

	void Break(void) const override
	{
		// esp_restart();
		while (1)
		{
			Delay(1);
		}
	}

	void Delay(uint16 Ms) const override
	{
		vTaskDelay(Ms / portTICK_PERIOD_MS);
	}

protected:
	void SetAnalogReadResolution(uint8 Value)
	{
		ASSERT(8 <= Value && Value <= 12, "Invalid Value");

		m_AnalogReadResolution = Value;
		m_MaxAnalogValue = 1 << Value;

		analogReadResolution(m_AnalogReadResolution);
	}

	uint8 GetAnalogReadResolution(void) const
	{
		return m_AnalogReadResolution;
	}

private:
	void InitializePWM(uint8 Pin)
	{
		PWMChannel *channel = FindOrGetPWMChannel(Pin);

		channel->Pin = Pin;
		channel->Used = true;

		ledcSetup(channel->Channel, PWM_FREQUENCY, m_PWMResolution);

		ledcAttachPin(Pin, channel->Channel);
	}

	PWMChannel *FindPWMChannel(uint8 Pin)
	{
		for (auto &pwmChannel : m_PWMChannels)
		{
			if (!pwmChannel.Used || pwmChannel.Pin != Pin)
				continue;

			return &pwmChannel;
		}

		return nullptr;
	}

	PWMChannel *FindOrGetPWMChannel(uint8 Pin)
	{
		PWMChannel *channel = FindPWMChannel(Pin);
		if (channel != nullptr)
			return channel;

		for (auto &pwmChannel : m_PWMChannels)
		{
			if (pwmChannel.Used)
				continue;

			return &pwmChannel;

			break;
		}

		ASSERT(false, "Out of PWM channel");
	}

private:
	PWMChannel m_PWMChannels[SOC_LEDC_CHANNEL_NUM];
	uint8 m_PWMResolution;
	uint32 m_PWMMaxDutyCycle;
	uint32 m_AnalogReadResolution;
	uint32 m_MaxAnalogValue;
};

#endif