#pragma once
#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include "Control.h"
#include "Memory.h"
#include "Task.h"
#include <vector>

class ControlManager
{
public:
	ControlManager(void)
	{
		Task::Create(
			[&]()
			{
				ProcessTask();
			},
			0, 1);
	}

	template <typename T, typename... ArgsT>
	T *Create(ArgsT... Args)
	{
		T *control = Memory::Allocate<T>();
		new (control) T(Args...);

		m_Controls.push_back(control);

		return control;
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
	}

private:
	std::vector<Control *> m_Controls;
};

#endif