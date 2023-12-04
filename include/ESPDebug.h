#pragma once
#ifndef ESP_DEBUG_H
#define ESP_DEBUG_H

#include "Debug.h"

#define ESP_CHECK_CALL(Expression) ASSERT((Expression) == ESP_OK, "Call Failed: %s", #Expression);

#endif