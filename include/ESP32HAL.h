#pragma once
#ifndef ESP32_HAL_H
#define ESP32_HAL_H

#include "DSP/IHAL.h"
#include "Common.h"
#include "DSP/Debug.h"
#include <Esp.h>
#include <EEPROM.h>
#include <nvs_flash.h>
#include <chrono>

using namespace std::chrono;

const int PWM_FREQUENCY = 240;
const int PWM_CHANNEL = 1;

template <uint16 PersistentSlotCount, uint8 PersistentSlotSize>
class ESP32HAL : public IHAL
{
	static_assert(PersistentSlotCount == 0 || PersistentSlotSize != 0, "PersistentSlotSize must be greater than zero");

private:
	struct PWMChannel
	{
	public:
		bool Used;
		uint8 Channel;
		uint8 Pin;
	};

	struct PersistentSlot
	{
	public:
		bool IsInitialized;
		uint8 Data[PersistentSlotSize];
	};

	struct PersistentData
	{
	public:
		uint8 Signature[10];
		PersistentSlot Data[PersistentSlotCount];
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

		if (PersistentSlotCount != 0)
		{
			nvs_flash_init();
			ASSERT(m_EEPROM.begin(sizeof(PersistentData)), "Couldn't initialize the EEPROM");
			m_EEPROM.readBytes(0, &m_PersistentData, sizeof(PersistentData));

			uint8 expected = 0;
			bool failed = false;
			for (const uint8 sign : m_PersistentData.Signature)
			{
				if (sign == expected++)
					continue;

				failed = true;
				break;
			}

			if (failed)
			{
				Memory::Set(&m_PersistentData, 0);

				expected = 0;
				for (uint8 &sign : m_PersistentData.Signature)
					sign = expected++;

				SavePersistentData();
			}
		}

		// I guess that's a lie that they mentioned in the overview of the ESP32-A1S which it has 4Mb PSRAM
		// printf("mem MALLOC_CAP_EXEC %i \n", heap_caps_get_free_size(MALLOC_CAP_EXEC));
		// printf("mem MALLOC_CAP_32BIT %i \n", heap_caps_get_free_size(MALLOC_CAP_32BIT));
		// printf("mem MALLOC_CAP_8BIT %i \n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
		// printf("mem MALLOC_CAP_DMA %i \n", heap_caps_get_free_size(MALLOC_CAP_DMA));
		// printf("mem MALLOC_CAP_PID2 %i \n", heap_caps_get_free_size(MALLOC_CAP_PID2));
		// printf("mem MALLOC_CAP_PID3 %i \n", heap_caps_get_free_size(MALLOC_CAP_PID3));
		// printf("mem MALLOC_CAP_PID4 %i \n", heap_caps_get_free_size(MALLOC_CAP_PID4));
		// printf("mem MALLOC_CAP_PID5 %i \n", heap_caps_get_free_size(MALLOC_CAP_PID5));
		// printf("mem MALLOC_CAP_PID6 %i \n", heap_caps_get_free_size(MALLOC_CAP_PID6));
		// printf("mem MALLOC_CAP_PID7 %i \n", heap_caps_get_free_size(MALLOC_CAP_PID7));
		// printf("mem MALLOC_CAP_SPIRAM %i \n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
		// printf("mem MALLOC_CAP_INTERNAL %i \n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
		// printf("mem MALLOC_CAP_DEFAULT %i \n", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
		// printf("mem MALLOC_CAP_IRAM_8BIT %i \n", heap_caps_get_free_size(MALLOC_CAP_IRAM_8BIT));
		// printf("mem MALLOC_CAP_RETENTION %i \n", heap_caps_get_free_size(MALLOC_CAP_RETENTION));
		// printf("mem MALLOC_CAP_RTCRAM %i \n", heap_caps_get_free_size(MALLOC_CAP_RTCRAM));
	}

	void *Allocate(uint32 Size, bool OnSDRAM = false) override
	{
		if (OnSDRAM)
			return heap_caps_malloc(Size, MALLOC_CAP_8BIT);

		return heap_caps_malloc(Size, MALLOC_CAP_DEFAULT);
	}

	void Deallocate(void *Memory) override
	{
		heap_caps_free(Memory);
	}

	bool IsAnAnaloglPin(uint8 Pin) const override
	{
		switch (Pin)
		{
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
		ASSERT((Mode != PinModes::AnalogInput && Mode != PinModes::DigitalInput) || IsAnInputPin(Pin), "Pin %i is not an input pin", Pin);
		ASSERT(Mode != PinModes::DigitalOutput || IsAnOutputPin(Pin), "Pin %i is not an output pin", Pin);
		ASSERT(Mode != PinModes::PWM || IsAPWMPin(Pin), "Pin %i is not an PWM pin", Pin);

		switch (Mode)
		{
		case PinModes::AnalogInput:
		case PinModes::DigitalInput:
			pinMode(Pin, INPUT_PULLDOWN);
			break;

		case PinModes::DigitalOutput:
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

	void InitializePersistentData(uint16 ID) override
	{
		ASSERT(PersistentSlotCount != 0, "PersistentSlotCount cannot be zero");

		PersistentSlot *slot = GetPersistentSlot(ID);
		ASSERT(!slot->IsInitialized, "Slot has already initialized");

		slot->IsInitialized = true;
	}

	bool ContainsPersistentData(uint16 ID) override
	{
		ASSERT(PersistentSlotCount != 0, "PersistentSlotCount cannot be zero");

		return GetPersistentSlot(ID)->IsInitialized;
	}

	void SetPersistentData(uint16 ID, const void *const Data, uint8 Size) override
	{
		ASSERT(PersistentSlotCount != 0, "PersistentSlotCount cannot be zero");
		ASSERT(Size <= PersistentSlotSize, "Size cannot be greater than PersistentSlotSize");

		PersistentSlot *slot = GetPersistentSlot(ID);
		ASSERT(slot->IsInitialized, "Slot hasn't initialized yet");

		Memory::Copy(reinterpret_cast<const uint8 *const>(Data), slot->Data, Size);
	}

	void GetPersistentData(uint16 ID, void *Data, uint8 Size) override
	{
		ASSERT(PersistentSlotCount != 0, "PersistentSlotCount cannot be zero");
		ASSERT(Size <= PersistentSlotSize, "Size cannot be greater than PersistentSlotSize");

		PersistentSlot *slot = GetPersistentSlot(ID);
		ASSERT(slot->IsInitialized, "Slot hasn't initialized yet");

		Memory::Copy(slot->Data, reinterpret_cast<uint8 *>(Data), Size);
	}

	void EreasPersistentData(void) override
	{
		ASSERT(PersistentSlotCount != 0, "PersistentSlotCount cannot be zero");

		Memory::Set(&m_PersistentData, 0);
		SavePersistentData();
	}

	void SavePersistentData(void) override
	{
		ASSERT(PersistentSlotCount != 0, "PersistentSlotCount cannot be zero");

		m_EEPROM.writeBytes(0, &m_PersistentData, sizeof(PersistentData));
		m_EEPROM.commit();
	}

	uint32 GetTimeSinceStartupMs(void) const override
	{
		return duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
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
			Delay(1000);
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

	PersistentSlot *GetPersistentSlot(uint16 ID)
	{
		ASSERT(ID <= PersistentSlotCount, "ID is out of bound of the PersistentSlotCount");

		return &m_PersistentData.Data[ID];
	}

private:
	PWMChannel m_PWMChannels[SOC_LEDC_CHANNEL_NUM];
	uint8 m_PWMResolution;
	uint32 m_PWMMaxDutyCycle;
	uint32 m_AnalogReadResolution;
	uint32 m_MaxAnalogValue;

	EEPROMClass m_EEPROM;
	PersistentData m_PersistentData;
};

#endif