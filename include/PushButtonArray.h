#pragma once
#ifndef LED_H
#define LED_H

#include "Memory.h"
#include <functional>
#include <Arduino.h>

class PushButtonArray
{
public:
	typedef std::function<void(void)> EventHandler;

private:
	class BindingInfo
	{
	public:
		bool AlreadyBound;
		int16 MinValue;
		int16 MaxValue;
		bool IsDown;
		EventHandler OnDown;
		EventHandler OnHold;
		EventHandler OnUp;
	};

public:
	PushButtonArray(GPIOPins Pin, uint8 ButtonCount)
		: m_Pin(Pin),
		  m_Bindings(nullptr),
		  m_BindingCount(ButtonCount)
	{
		uint16 diff = 1024 / (m_BindingCount + 1);

		m_Bindings = Memory::Allocate<BindingInfo>(m_BindingCount);
		for (uint8 i = 0; i < m_BindingCount; ++i)
		{
			BindingInfo &info = m_Bindings[i];

			info.MinValue = (i + 1) * diff;
			info.MaxValue = ((i + 2) * diff) - 1;
		}

		pinMode((uint8)m_Pin, INPUT);
	}

	void Bind(uint8 Index, EventHandler &&OnDown, EventHandler &&OnHold, EventHandler &&OnUp)
	{
		ASSERT(Index < m_BindingCount, TAG, "Index is out of range");

		BindingInfo &info = m_Bindings[Index];
		ASSERT(!info.AlreadyBound, TAG, "There is already a binding to this index");

		info.AlreadyBound = true;
		info.OnDown = OnDown;
		info.OnHold = OnHold;
		info.OnUp = OnUp;
	}

	void Update(void)
	{
		int32 value = analogRead((uint8)m_Pin);

		for (uint8 i = 0; i < m_BindingCount; ++i)
		{
			BindingInfo &info = m_Bindings[i];

			if (info.MinValue <= value && value <= info.MaxValue)
				continue;

			if (!info.IsDown)
				continue;

			info.IsDown = false;

			if (info.OnUp != nullptr)
				info.OnUp();
		}

		for (uint8 i = 0; i < m_BindingCount; ++i)
		{
			BindingInfo &info = m_Bindings[i];

			if (value < info.MinValue || info.MaxValue < value)
				continue;

			if (info.IsDown)
			{
				if (info.OnHold != nullptr)
					info.OnHold();
			}
			else
			{
				info.IsDown = true;

				if (info.OnDown != nullptr)
					info.OnDown();
			}

			break;
		}
	}

private:
	GPIOPins m_Pin;
	BindingInfo *m_Bindings;
	uint8 m_BindingCount;
	static const char *TAG;
};

const char *PushButtonArray::TAG = "PushButtonArray";

#endif