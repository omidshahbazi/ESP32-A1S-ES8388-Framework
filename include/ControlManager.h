#pragma once
#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include "IControl.h"
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
				UpdateTask();
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
	void UpdateTask(void)
	{
		while (true)
		{
			Task::Delay(1);

			for (IControl *control : m_Controls)
				control->Update();
		}
	}

private:
	std::vector<IControl *> m_Controls;
};

#endif