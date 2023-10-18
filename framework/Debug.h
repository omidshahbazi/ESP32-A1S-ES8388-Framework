#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include "Common.h"
#include <esp_log.h>
#include <esp_system.h>

template <typename... ArgsT>
void Print(esp_log_level_t Level, const char *Tag, const char *Message, ArgsT... Args)
{
	char buff[100];
	int32 len = snprintf(buff, sizeof(buff), Message, Args...);
	buff[len] = '\n';
	buff[len + 1] = '\0';

	printf(buff);
}

#define LOG_INFO(Tag, Message, ...)                       \
	do                                                    \
	{                                                     \
		Print(ESP_LOG_INFO, Tag, Message, ##__VA_ARGS__); \
	} while (false)

#define LOG_WARNING(Tag, Message, ...)                    \
	do                                                    \
	{                                                     \
		Print(ESP_LOG_WARN, Tag, Message, ##__VA_ARGS__); \
	} while (false)

#define LOG_ERROR(Tag, Message, ...)                       \
	do                                                     \
	{                                                      \
		Print(ESP_LOG_ERROR, Tag, Message, ##__VA_ARGS__); \
	} while (false)

#define ASSERT(Expression, Tag, Message, ...)   \
	do                                          \
	{                                           \
		if (Expression)                         \
			break;                              \
		LOG_ERROR(Tag, Message, ##__VA_ARGS__); \
		esp_restart();                          \
	} while (false)

#define CHECK_CALL(Expression) ASSERT(Expression, "CHECK_CALL", "Call Failed: %s", #Expression);

#endif