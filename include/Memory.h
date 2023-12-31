#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include "Common.h"
#include "Debug.h"
#include <memory.h>
#include <Esp.h>

class Memory
{
public:
	template <typename T>
	static T *Allocate(uint16 Count = 1, bool FromExternalRAM = false)
	{
		uint16 length = sizeof(T) * Count;

		uint32 ramType = MALLOC_CAP_DEFAULT;
		// if (FromExternalRAM)
		// 	ramType = MALLOC_CAP_SPIRAM;

		T *mem = reinterpret_cast<T *>(heap_caps_malloc(length, ramType));

		ASSERT(mem != nullptr, "Couldn't allocate memory: %i of %iB", Count, sizeof(T));

		Log::WriteDebug("Memory", "%ib Allocated Count: %i, ELement Size: %i, Available RAM: %ib", length, Count, sizeof(T), ESP.getFreeHeap());

		Set(mem, 0, Count);

		return mem;
	}

	template <typename T>
	static void Deallocate(T *Memory)
	{
		heap_caps_free(Memory);
	}

	template <typename T>
	static void Set(T *Memory, int32 Value, uint16 Count = 1)
	{
		memset(Memory, Value, sizeof(T) * Count);
	}

	template <typename T>
	static void Copy(const T *Source, T *Destination, uint16 Count = 1)
	{
		memcpy(Destination, Source, sizeof(T) * Count);
	}
};
#endif