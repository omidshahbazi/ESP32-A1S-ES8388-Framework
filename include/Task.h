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
	static void Create(EntrypointType &&Entrypoint, uint32 StackDepth, const char *Name = nullptr, uint8 CoreID = 1, uint8 Priority = 1, bool FromExternalRAM = false)
	{
		if (Name == nullptr)
			Name = "Unknown";

		uint32 stackSizeInWords = StackDepth / 4;

		TaskInfo *taskInfo = Memory::Allocate<TaskInfo>(1, true);
		taskInfo->Entrypoint = Entrypoint;
		BaseType_t result = xTaskCreatePinnedToCore(Stub, Name, stackSizeInWords, taskInfo, Priority, nullptr, CoreID);

		ASSERT(result == pdPASS, "Task", "Didn't manage to create the %s on Core %i with Priority of %i and the StackDepth of %ib", Name, CoreID, Priority, stackSizeInWords);
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
		EntrypointType entryPoint = reinterpret_cast<TaskInfo *>(Args)->Entrypoint;
		Memory::Deallocate(Args);
		entryPoint();
	}
};
#endif