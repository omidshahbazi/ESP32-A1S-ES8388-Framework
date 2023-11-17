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

	uint16 AnalogRead(void) const
	{
		return analogRead((uint8)m_Pin);
	}

	void DigitalWrite(bool Value)
	{
		digitalWrite((uint8)m_Pin, Value);
	}

	bool DigitalRead(void) const
	{
		return digitalRead((uint8)m_Pin);
	}

private:
	GPIOPins m_Pin;
	bool m_Enabled;
};

#endif