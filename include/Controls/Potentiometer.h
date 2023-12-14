#pragma once
#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "Control.h"
#include "../Log.h"
// #include "../Filters/BiquadFilter.h"
#include <functional>

class Potentiometer : public Control
{
public:
	typedef std::function<void(float)> EventHandler;

public:
	Potentiometer(GPIOPins Pin)
		: Control(Pin, Modes::Input),
		  //   m_Filter(1),
		  m_Value(-1)
	{
		// BiquadFilter::SetLowPassFilterCoefficients(&m_Filter, 1000, 4, 1000, 10);
	}

	float GetValue(void) const
	{
		return m_Value;
	}

	void SetOnChangedListener(EventHandler &&Listener)
	{
		m_OnChanged = Listener;
	}

protected:
	void Update(void) override
	{
		uint16 value = AnalogRead();

		float prevValue = m_Value;

		m_Value = value / 1023.0F;

		// m_Value = m_Filter.Process(m_Value);
		// printf("%f#%f\n", m_Value, prevValue);

		if (abs(prevValue - m_Value) >= 0.01F)
		{

			Log::WriteDebug("Potentiometer", "Potentiometer GPIOPins::Pin%i value: %f, diff %f", (uint8)GetPin(), m_Value, abs(prevValue - m_Value));

			if (m_OnChanged != nullptr)
				m_OnChanged(m_Value);
		}
	}

private:
	// BiquadFilter m_Filter;
	float m_Value;
	EventHandler m_OnChanged;
};

#endif