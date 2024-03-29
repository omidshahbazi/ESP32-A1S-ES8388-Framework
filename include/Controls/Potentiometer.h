#pragma once
#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "Control.h"
#include "../DSP/Log.h"
#include "../DSP/Filters/BiquadFilter.h"
#include <functional>

class Potentiometer : public Control
{
public:
	typedef std::function<void(float)> EventHandler;

public:
	Potentiometer(GPIOPins Pin)
		: Control(Pin, Modes::Input),
		  m_Filter(1),
		  m_Value(-1)
	{
		BiquadFilter::SetLowPassFilterCoefficients(&m_Filter, PROCESS_RATE, 0.5);
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

		if (0 < m_Value && m_Value < 1)
			m_Value = Math::Clamp01(m_Filter.Process(m_Value));

		if (abs(prevValue - m_Value) >= 0.005F)
		{
			Log::WriteDebug("Potentiometer", "Potentiometer GPIOPins::Pin%i value: %f, diff %f", (uint8)GetPin(), m_Value, abs(prevValue - m_Value));

			if (m_OnChanged != nullptr)
				m_OnChanged(m_Value);
		}
	}

private:
	BiquadFilter m_Filter;
	float m_Value;
	EventHandler m_OnChanged;
};

#endif