#pragma once
#ifndef LED_H
#define LED_H

#include "Control.h"
#include "../Time.h"
#include "../Math.h"

class LED : public Control
{
public:
	LED(GPIOPins Pin)
		: Control(Pin, Modes::Output),
		  m_TurnedOn(false),
		  m_IsBlinking(false),
		  m_BlinkRate(0),
		  m_NextBlinkTime(0)
	{
	}

	void SetTurnedOn(bool Value)
	{
		m_IsBlinking = false;

		m_TurnedOn = Value;

		UpdatePin();
	}
	bool GetTurnedOn(void) const
	{
		return m_TurnedOn;
	}

	void Toggle(bool Value)
	{
		m_IsBlinking = false;

		m_TurnedOn = !m_TurnedOn;

		UpdatePin();
	}

	//(0, 10]
	void SetBlinking(float BlinkRate)
	{
		ASSERT(0 < BlinkRate && BlinkRate <= 1000, "Invalid BlinkRate");

		m_IsBlinking = true;
		m_BlinkRate = BlinkRate;
	}
	bool GetBlinking(void) const
	{
		return m_IsBlinking;
	}

protected:
	void Update(void) override
	{
		if (!m_IsBlinking)
			return;

		if (m_NextBlinkTime > Time::Now())
			return;

		m_NextBlinkTime += 1 / m_BlinkRate;

		m_TurnedOn = !m_TurnedOn;

		UpdatePin();
	}

private:
	void UpdatePin(void)
	{
		DigitalWrite(m_TurnedOn);
	}

private:
	bool m_TurnedOn;
	bool m_IsBlinking;
	float m_BlinkRate;
	float m_NextBlinkTime;
};

#endif