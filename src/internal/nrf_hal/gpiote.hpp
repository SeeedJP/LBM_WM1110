/*
 * gpiote.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <Arduino.h>
#include "types.hpp"

////////////////////////////////////////////////////////////////////////////////
// Namespace

namespace nrf_hal
{

////////////////////////////////////////////////////////////////////////////////
// GpioteEventInterrupt

template <uint32_t ARDUINO_PIN>
class GpioteEventInterrupt
{
private:
    static IsrAndContext isr_;

private:
    static void eventIsr()
    {
        if (isr_.Isr == nullptr) abort();

        isr_.Isr(isr_.Context);
    }

public:
    static void begin(uint32_t mode, void (*callback)(void* context), void* context)
    {
        if (callback == nullptr) abort();

        isr_ = { callback, context };

        attachInterrupt(ARDUINO_PIN, eventIsr, mode);
    }

    static void enableIRQ()
    {
        enableInterruptIRQ(ARDUINO_PIN);
    }

    static void disableIRQ()
    {
        disableInterruptIRQ(ARDUINO_PIN);
    }

    static void clearIRQ()
    {
        clearInterruptIRQ(ARDUINO_PIN);
    }

};

template <uint32_t ARDUINO_PIN>
IsrAndContext GpioteEventInterrupt<ARDUINO_PIN>::isr_;

////////////////////////////////////////////////////////////////////////////////
// Namespace

}

////////////////////////////////////////////////////////////////////////////////
