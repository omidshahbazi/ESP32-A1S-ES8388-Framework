#pragma once
#ifndef CONTROL_H
#define CONTROL_H

#include "../Common.h"
#include <Arduino.h>

class Control
{
protected:
	enum class Modes
	{
		Input = 0,
		Output
	};

public:
	Control(GPIOPins Pin, Modes Mode)
		: m_Pin(Pin),
		  m_Enabled(true)
	{
		pinMode((uint8)m_Pin, (Mode == Modes::Input ? INPUT : OUTPUT));

		analogReadResolution(10);
	}

	void SetEnabled(bool Value)
	{
		m_Enabled = false;
	}
	bool GetEnabled(void) const
	{
		return m_Enabled;
	}

	void Process(void)
	{
		if (!m_Enabled)
			return;

		Update();
	}

protected:
	virtual void Update(void) = 0;

	GPIOPins GetPin(void) const
	{
		return m_Pin;
	}

	uint16 AnalogRead(void) const
	{
		return analogRead((uint8)m_Pin);
	}

	bool DigitalRead(void) const
	{
		return (digitalRead((uint8)m_Pin) != LOW);
	}

	void DigitalWrite(bool Value)
	{
		digitalWrite((uint8)m_Pin, Value);
	}

private:
	GPIOPins m_Pin;
	bool m_Enabled;
};

#endif