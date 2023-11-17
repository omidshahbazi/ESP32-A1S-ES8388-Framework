#pragma once
#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "Control.h"
#include <functional>

class Potentiometer : public Control
{
public:
	typedef std::function<void(float)> EventHandler;

public:
	Potentiometer(GPIOPins Pin)
		: Control(Pin, Modes::Input),
		  m_Value(0)
	{
	}

	float GetValue(void) const
	{
		return m_Value;
	}

	void SetOnChangedListener(EventHandler &&Listener)
	{
		m_OnChanged = Listener;
	}

	void Update(void) override
	{
		uint16 value = AnalogRead();

		float prevValue = m_Value;

		m_Value = value / 1023.0F;

		if (abs(prevValue - m_Value) >= 0.01F)
			if (m_OnChanged != nullptr)
				m_OnChanged(m_Value);
	}

private:
	float m_Value;
	EventHandler m_OnChanged;
};

#endif