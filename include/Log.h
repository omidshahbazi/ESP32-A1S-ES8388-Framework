#pragma once
#ifndef LOG_H
#define LOG_H

#include "Bitwise.h"
#include <stdio.h>

class Log
{
public:
	enum class Types
	{
		None = 0,
		Debug = 1,
		Info = 2,
		Warning = 4,
		Error = 8,
		Critical = 16,
		General = Info | Warning | Error | Critical,
		All = Debug | Info | Warning | Error | Critical
	};

public:
	static void SetMask(Types Mask)
	{
		m_Mask = Mask;
	}

	template <typename... ArgsT>
	static void Write(Types Type, const char *Tag, const char *FormattedMessage, ArgsT... Args)
	{
		if (!Bitwise::IsEnabled(m_Mask, Type))
			return;

		if (FormattedMessage == nullptr)
			return;

		const int16 SIZE = 512;
		char buff[SIZE];

		int32 index = 0;

		index += snprintf(buff + index, SIZE, "[");
		if (Bitwise::IsEnabled(Type, Types::Debug))
			index += snprintf(buff + index, SIZE, "D");
		if (Bitwise::IsEnabled(Type, Types::Info))
			index += snprintf(buff + index, SIZE, "I");
		if (Bitwise::IsEnabled(Type, Types::Warning))
			index += snprintf(buff + index, SIZE, "W");
		if (Bitwise::IsEnabled(Type, Types::Error))
			index += snprintf(buff + index, SIZE, "E");
		if (Bitwise::IsEnabled(Type, Types::Critical))
			index += snprintf(buff + index, SIZE, "C");
		index += snprintf(buff + index, SIZE, "] ");

		if (Tag != nullptr)
			index += snprintf(buff + index, SIZE, "[%s] ", Tag);

		index += snprintf(buff + index, SIZE, FormattedMessage, Args...);

		index += snprintf(buff + index, SIZE, "\n");

		buff[index] = '\0';

		printf(buff);
	}

	template <typename... ArgsT>
	static void WriteDebug(const char *FormattedMessage, ArgsT... Args)
	{
		Write(Types::Debug, nullptr, FormattedMessage, Args...);
	}

	template <typename... ArgsT>
	static void WriteDebug(const char *Tag, const char *FormattedMessage, ArgsT... Args)
	{
		Write(Types::Debug, Tag, FormattedMessage, Args...);
	}

	template <typename... ArgsT>
	static void WriteInfo(const char *FormattedMessage, ArgsT... Args)
	{
		Write(Types::Info, nullptr, FormattedMessage, Args...);
	}

	template <typename... ArgsT>
	static void WriteInfo(const char *Tag, const char *FormattedMessage, ArgsT... Args)
	{
		Write(Types::Info, Tag, FormattedMessage, Args...);
	}

	template <typename... ArgsT>
	static void WriteWarning(const char *FormattedMessage, ArgsT... Args)
	{
		Write(Types::Warning, nullptr, FormattedMessage, Args...);
	}

	template <typename... ArgsT>
	static void WriteWarning(const char *Tag, const char *FormattedMessage, ArgsT... Args)
	{
		Write(Types::Warning, Tag, FormattedMessage, Args...);
	}

	template <typename... ArgsT>
	static void WriteError(const char *FormattedMessage, ArgsT... Args)
	{
		Write(Types::Error, nullptr, FormattedMessage, Args...);
	}

	template <typename... ArgsT>
	static void WriteError(const char *Tag, const char *FormattedMessage, ArgsT... Args)
	{
		Write(Types::Error, Tag, FormattedMessage, Args...);
	}

	template <typename... ArgsT>
	static void WriteCritical(const char *FormattedMessage, ArgsT... Args)
	{
		Write(Types::Critical, nullptr, FormattedMessage, Args...);
	}

	template <typename... ArgsT>
	static void WriteCritical(const char *Tag, const char *FormattedMessage, ArgsT... Args)
	{
		Write(Types::Critical, Tag, FormattedMessage, Args...);
	}

private:
	static Types m_Mask;
};

Log::Types Log::m_Mask = Log::Types::None;

#endif