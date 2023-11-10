#pragma once
#ifndef LED_H
#define LED_H

#include "Common.h"
#include "Time.h"
#include <Arduino.h>

class LED
{
public:
	LED(GPIOPins Pin)
		: m_Pin(Pin),
		  m_TurnedOn(false),
		  m_IsBlinking(false),
		  m_BlinkRate(0),
		  m_NextBlinkTime(0)
	{
		pinMode((uint8)m_Pin, OUTPUT);
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

	void SetBlinking(float BlinkRate)
	{
		m_IsBlinking = true;
		m_BlinkRate = Math::Clamp(BlinkRate, 0.0001F, 1000);
	}
	bool GetBlinking(void) const
	{
		return m_IsBlinking;
	}

	void Update(void)
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
		digitalWrite((uint8)m_Pin, m_TurnedOn);
	}

private:
	GPIOPins m_Pin;
	bool m_TurnedOn;
	bool m_IsBlinking;
	float m_BlinkRate;
	float m_NextBlinkTime;
};

#endif