#pragma once
#ifndef CONTROL_FACTORY_H
#define CONTROL_FACTORY_H

#include "Control.h"
#include "../Memory.h"
#include "../Task.h"
#include <vector>

class ControlFactory
{
public:
	ControlFactory(void)
	{
		Task::Create(
			[&]()
			{
				ProcessTask();
			},
			2048, "ControlsTask", 0, 1);
	}

	template <typename T, typename... ArgsT>
	T *Create(ArgsT... Args)
	{
		T *control = Memory::Allocate<T>(1, true);
		new (control) T(Args...);

		m_Controls.push_back(control);

		return control;
	}

	template <typename T>
	void Destroy(T *Control)
	{
		m_Controls.erase(std::find(m_Controls.begin, m_Controls.end, Control));

		Memory::Deallocate(Control);
	}

private:
	void ProcessTask(void)
	{
		while (true)
		{
			Task::Delay(1);

			for (Control *control : m_Controls)
				control->Process();
		}

		Task::Delete();
	}

private:
	std::vector<Control *> m_Controls;
};

#endif