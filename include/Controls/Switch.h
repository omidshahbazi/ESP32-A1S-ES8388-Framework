#pragma once
#ifndef SWITCH_H
#define SWITCH_H

#include "Control.h"
#include <functional>

class Switch : public Control
{
public:
	typedef std::function<void(bool)> EventHandler;

public:
	Switch(GPIOPins Pin)
		: Control(Pin, Modes::Input),
		  m_TurnedOn(false)
	{
		Update();
	}

	void SetOnChangedListener(EventHandler &&Listener)
	{
		m_OnChanged = Listener;
	}

	bool GetTurnedOn(void) const
	{
		return m_TurnedOn;
	}

protected:
	void Update(void) override
	{
		bool newValue = DigitalRead();
		if (m_TurnedOn == newValue)
			return;

		m_TurnedOn = newValue;

		if (m_OnChanged != nullptr)
			m_OnChanged(m_TurnedOn);
	}

private:
	bool m_TurnedOn;
	EventHandler m_OnChanged;
};

#endif