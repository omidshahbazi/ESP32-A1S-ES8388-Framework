#pragma once
#ifndef OVERDRIVE_H
#define OVERDRIVE_H

#include "IDSP.h"
#include "../Math.h"
#include "../Debug.h"

class Overdrive : public IDSP
{
public:
	Overdrive(void)
		: m_Drive(0)
	{
		SetDrive(1);
	}

	//[0, 1]
	void SetDrive(float Value)
	{
		ASSERT(0 <= Value && Value <= 1, "Overdrive", "Invalid Value");

		m_Drive = Value;
	}
	float GetDrive(void)
	{
		return m_Drive;
	}

	void ProcessBuffer(double *Buffer, uint16 Count) override
	{
		for (uint16 i = 0; i < Count; ++i)
		{
			double value = ExponentialSaturation(Buffer[i], m_Drive);

			Buffer[i] = value;
		}
	}

private:
	static double Soft1Clip(double Value, float Drive)
	{
		return tanh(Value * (2 + (Drive * 50)));
	}

	static double Soft2Clip(double Value, float Drive)
	{
		return atan(Value * (Drive + 1)) * (Drive + 1);
	}

	static double HardClip(double Value, float Drive)
	{
		return (Value < -Drive ? -1 : (Value > Drive ? 1 : Value));
	}

	static double ExponentialSaturation(double Value, float Drive)
	{
		// return exp(Drive * fabs(Value)) * Math::Sign(Value) * 0.37;
		return exp(Drive * 4.6 * fabs(Value)) * Math::Sign(Value) * 0.01;
	}

	static double AsymmetricSineSaturation(double Value, float Drive)
	{
		return sin(Value * M_PI / 2) * (Drive + 1);
	}

	static double FoldbackDistortion(double Value, float Drive)
	{
		return (Value < -Drive || Drive < Value) ? Value : -1;
	}

private:
	float m_Drive;
};

#endif