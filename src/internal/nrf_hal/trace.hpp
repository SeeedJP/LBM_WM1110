/*
 * trace.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <SEGGER_RTT.h>

////////////////////////////////////////////////////////////////////////////////
// Namespace

namespace nrf_hal
{

////////////////////////////////////////////////////////////////////////////////
// Trace

class Trace
{
private:
    static constexpr unsigned int BUFFER_ID = 0;

public:
    Trace() = delete;

    static void vprintf(const char* format, va_list args)
    {
        if (format == nullptr || format[0] == '\0') return;

        char str[WM1110_TRACE_MAX_LENGTH];

        const int len = vsnprintf(str, sizeof(str), format, args);
        if (len < 0) return;    // Error
        if (len == 0) return;   // Empty string

        // Overlength
        if (len > static_cast<int>(sizeof(str) - 1))
        {
            if (sizeof(str) < 3) abort();

            str[sizeof(str) - 3] = '~';
            str[sizeof(str) - 2] = '\n';
            str[sizeof(str) - 1] = '\0';
        }

        SEGGER_RTT_Write(BUFFER_ID, str, len);
    }

    static void printf(const char* format, ...)
    {
        va_list args;

        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }

};

////////////////////////////////////////////////////////////////////////////////
// Namespace

}

////////////////////////////////////////////////////////////////////////////////
