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
	static void Create(EntrypointType &&Entrypoint, uint32 StackSize, const char *Name = nullptr, uint8 CoreID = 0, uint8 Priority = 1)
	{
		if (Name == nullptr)
			Name = "Unknown";

		TaskInfo *taskInfo = Memory::Allocate<TaskInfo>();
		taskInfo->Entrypoint = Entrypoint;
		BaseType_t result = xTaskCreatePinnedToCore(Stub, Name, StackSize, taskInfo, Priority, nullptr, CoreID);

		ASSERT(result == pdPASS, "Didn't manage to create the %s on Core %i with Priority of %i and the StackDepth of %ib", Name, CoreID, Priority, StackSize);
	}

	static void Delete(void)
	{
		vTaskDelete(nullptr);
	}

private:
	static void Stub(void *Args)
	{
		reinterpret_cast<TaskInfo *>(Args)->Entrypoint();
	}
};
#endif