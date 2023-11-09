#pragma once
#ifndef TASK_H
#define TASK_H

#include "Memory.h"
#include <task_snapshot.h>
#include <functional>

class Task
{
public:
	typedef std::function<void(void)> EntrypointType;

private:
	class TaskInfo
	{
	public:
		EntrypointType Entrypoint;
	};

public:
	static void Create(EntrypointType &&Entrypoint, uint8 CoreID = 1, uint8 Priority = 1, uint16 StackDepth = 4096)
	{
		TaskInfo *taskInfo = Memory::Allocate<TaskInfo>();
		taskInfo->Entrypoint = Entrypoint;

		xTaskCreatePinnedToCore(Stub, "PassthroughTask", StackDepth, taskInfo, Priority, nullptr, CoreID);
	}

	static void Delete(void)
	{
		vTaskDelete(nullptr);
	}

	static void Delay(uint16 Ms)
	{
		vTaskDelay(Ms / portTICK_PERIOD_MS);
	}

private:
	static void Stub(void *Args)
	{
		TaskInfo *taskInfo = reinterpret_cast<TaskInfo *>(Args);
		taskInfo->Entrypoint();
	}
};
#endif