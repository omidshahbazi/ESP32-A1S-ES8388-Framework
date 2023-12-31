#pragma once
#ifndef PUSH_BUTTON_ARRAY_H
#define PUSH_BUTTON_ARRAY_H

#include "Control.h"
#include "../Memory.h"
#include <functional>

class PushButtonArray : public Control
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
		: Control(Pin, Modes::Input),
		  m_Bindings(nullptr),
		  m_BindingCount(ButtonCount)
	{
		float diff = 1024.0F / (m_BindingCount + 1);

		m_Bindings = Memory::Allocate<BindingInfo>(m_BindingCount, true);
		for (uint8 i = 0; i < m_BindingCount; ++i)
		{
			BindingInfo &info = m_Bindings[i];

			info.AlreadyBound = false;
			info.MinValue = (i + 1) * diff;
			info.MaxValue = ((i + 2) * diff) - 1;
			info.IsDown = false;
			info.OnDown = nullptr;
			info.OnHold = nullptr;
			info.OnUp = nullptr;
		}
	}

	void Bind(uint8 Index, EventHandler &&OnDown, EventHandler &&OnHold, EventHandler &&OnUp)
	{
		ASSERT(Index < m_BindingCount, "Index is out of range");

		BindingInfo &info = m_Bindings[Index];
		ASSERT(!info.AlreadyBound, "There is already a binding to this index");

		info.AlreadyBound = true;
		info.OnDown = OnDown;
		info.OnHold = OnHold;
		info.OnUp = OnUp;
	}

protected:
	void Update(void) override
	{
		uint16 value = AnalogRead();

		bool aButtonBacksUp = false;
		for (uint8 i = 0; i < m_BindingCount; ++i)
		{
			BindingInfo &info = m_Bindings[i];

			if (!info.AlreadyBound)
				continue;

			if (info.MinValue <= value && value <= info.MaxValue)
				continue;

			if (!info.IsDown)
				continue;

			info.IsDown = false;

			if (info.OnUp != nullptr)
				info.OnUp();

			aButtonBacksUp = true;
		}

		if (aButtonBacksUp)
			return;

		for (uint8 i = 0; i < m_BindingCount; ++i)
		{
			BindingInfo &info = m_Bindings[i];

			if (!info.AlreadyBound)
				continue;

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
	BindingInfo *m_Bindings;
	uint8 m_BindingCount;
};

#endif