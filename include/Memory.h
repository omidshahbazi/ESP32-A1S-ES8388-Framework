#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include "Common.h"
#include "Debug.h"
#include <memory.h>

class Memory
{
public:
	template <typename T>
	static T *Allocate(uint16 Count = 1)
	{
		T *mem = reinterpret_cast<T *>(malloc(sizeof(T) * Count));

		ASSERT(mem != nullptr, "Memory", "Couldn't allocate memory: %i of %iB", Count, sizeof(T));

		Set(mem, 0, Count);

		return mem;
	}

	template <typename T>
	static void Deallocate(T *Memory)
	{
		delete[] Memory;
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