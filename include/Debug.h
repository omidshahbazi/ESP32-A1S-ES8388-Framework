#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include "Log.h"
#include <esp_system.h>

#define STRINGIZE(Value) #Value
#define STRINGIZE_NUMBER(Value) STRINGIZE(Value)

#if __PLATFORMIO_BUILD_DEBUG__
#define ASSERT(Expression, Message, ...)                                                                        \
	do                                                                                                          \
	{                                                                                                           \
		if (Expression)                                                                                         \
			break;                                                                                              \
		Log::WriteCritical(__FILE__ ":Ln" STRINGIZE_NUMBER(__LINE__) ", " #Expression, Message, ##__VA_ARGS__); \
		esp_restart();                                                                                          \
	} while (false)
#else
#define ASSERT(Expression, Message, ...) \
	do                                   \
	{                                    \
		Expression;                      \
	} while (false)
#endif

#define CHECK_CALL(Expression) ASSERT(Expression, "CallFailed")

#endif