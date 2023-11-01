#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include "Common.h"
#include "Debug.h"
#include <memory.h>

template <typename T>
T *Allocate(uint16 Count = 1)
{
	T *mem = reinterpret_cast<T *>(malloc(sizeof(T) * Count));

	ASSERT(mem != nullptr, "Memory", "Couldn't allocate memory: %i of %iB", Count, sizeof(T));

	memset(mem, 0, sizeof(T) * Count);

	return mem;
}

#endif