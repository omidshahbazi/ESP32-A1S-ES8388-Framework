#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include "Log.h"
#include <esp_system.h>
// #include "Plotter/Plotter.h"

class Debug
{
public:
	static void Initialize(void)
	{
#if __PLATFORMIO_BUILD_DEBUG__
		// GetPlotter().Begin();
#endif
	}

	template <typename A>
	static void AddGraph(const char *Title, int PointsDisplayed, const char *Label, A &Value)
	{
#if __PLATFORMIO_BUILD_DEBUG__
		// GetPlotter().AddTimeGraph(Title, PointsDisplayed, Label, Value);
#endif
	}

	static void Plot(void)
	{
#if __PLATFORMIO_BUILD_DEBUG__
		// GetPlotter().Plot();
#endif
	}

private:
	// static Plotter &GetPlotter(void)
	// {
	// 	static Plotter plotter;

	// 	return plotter;
	// }
};

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