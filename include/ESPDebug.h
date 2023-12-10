#pragma once
#ifndef ESP_DEBUG_H
#define ESP_DEBUG_H

#include "Debug.h"

#define ESP_CHECK_CALL(Expression)                                                     \
	do                                                                                 \
	{                                                                                  \
		esp_err_t returnValue = Expression;                                            \
		if (returnValue == ESP_OK)                                                     \
			break;                                                                     \
		ASSERT(Expression, "Call Failed Because of %s", esp_err_to_name(returnValue)); \
	} while (false)

#endif