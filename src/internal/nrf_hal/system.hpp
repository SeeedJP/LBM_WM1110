/*
 * system.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdbool>
#include <cstdint>
#include <cstdlib>
#include <nrf_sdm.h>
#include <nrf_nvic.h>
#include <nrf_soc.h>
#include <nrf_clock.h>
#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////
// Namespace

namespace nrf_hal
{

////////////////////////////////////////////////////////////////////////////////
// System

class System
{
public:
    System() = delete;

#ifdef SOFTDEVICE_PRESENT
    static bool isSoftDeviceEnabled()
    {
        uint8_t enabled;
        if (sd_softdevice_is_enabled(&enabled) != NRF_SUCCESS) abort();

        return enabled == 1;
    }
#endif

    static void reset()
    {
#ifdef SOFTDEVICE_PRESENT
        if (isSoftDeviceEnabled())
        {
            sd_nvic_SystemReset();
        }
        else
#endif
        {
            NVIC_SystemReset();
        }
    }

    static void delayMs(uint32_t milliseconds)
    {
        delay(milliseconds);
    }

    static void __attribute__((optimize("O0"))) SpinDelayUs(uint32_t microseconds)
    {
        const uint32_t n = microseconds * 64 / 12;  // microseconds * 64MHz / 12cycles
        for (uint32_t i = 0; i < n; ++i)
        {
            __NOP();
        } 
    }

    static void startHfclk()
    {
#ifdef SOFTDEVICE_PRESENT
        if (isSoftDeviceEnabled())
        {
            uint32_t running;
            if (sd_clock_hfclk_is_running(&running) != NRF_SUCCESS) abort();
            if (running != 1)
            {
                if (sd_clock_hfclk_request() != NRF_SUCCESS) abort();
            }
        }
        else
#endif
        {
            nrf_clock_hfclk_t clockSrc;
            if (!nrf_clock_is_running(NRF_CLOCK, NRF_CLOCK_DOMAIN_HFCLK, &clockSrc) || clockSrc != NRF_CLOCK_HFCLK_HIGH_ACCURACY)
            {
                nrf_clock_event_clear(NRF_CLOCK, NRF_CLOCK_EVENT_HFCLKSTARTED);
                nrf_clock_task_trigger(NRF_CLOCK, NRF_CLOCK_TASK_HFCLKSTART);
            }
        }
    }

};

////////////////////////////////////////////////////////////////////////////////
// Namespace

}

////////////////////////////////////////////////////////////////////////////////
