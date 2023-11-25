#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include "Log.h"
#include <esp_system.h>

#if __PLATFORMIO_BUILD_DEBUG__
#define ASSERT(Expression, Tag, Message, ...)            \
	do                                                   \
	{                                                    \
		if (Expression)                                  \
			break;                                       \
		Log::WriteCritical(Tag, Message, ##__VA_ARGS__); \
		esp_restart();                                   \
	} while (false)
#else
#define ASSERT(Expression, Tag, Message, ...) \
	do                                        \
	{                                         \
		Expression;                           \
	} while (false)
#endif

#define CHECK_CALL(Expression) ASSERT(Expression, "CHECK_CALL", "Call Failed: %s", #Expression)

#endif