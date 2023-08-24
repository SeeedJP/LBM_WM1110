/*
 * wdt.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdlib>
#include <nrfx_wdt.h>
#include "system.hpp"

////////////////////////////////////////////////////////////////////////////////
// Namespace

namespace nrf_hal
{

////////////////////////////////////////////////////////////////////////////////
// Wdt

template <int ID>
class Wdt
{
private:
    static nrfx_wdt_t wdt_;

private:
    static void wdtIsr()
    {
        nrf_hal::System::reset();
    }

public:
    Wdt()
    {
#if NRFX_CHECK(NRFX_WDT0_ENABLED)
        if (ID == 0) { wdt_ = NRFX_WDT_INSTANCE(0); return; }
#endif
        abort();
    }

    static void begin()
    {
        nrfx_wdt_config_t config = NRFX_WDT_DEFAULT_CONFIG;
        if (nrfx_wdt_init(&wdt_, &config, wdtIsr) != NRFX_SUCCESS) abort();

        nrfx_wdt_channel_id channelId;
        if (nrfx_wdt_channel_alloc(&wdt_, &channelId) != NRFX_SUCCESS) abort();

        nrfx_wdt_enable(&wdt_);
    }

    static void reload()
    {
        nrfx_wdt_feed(&wdt_);
    }

};

template <int ID>
nrfx_wdt_t Wdt<ID>::wdt_;

////////////////////////////////////////////////////////////////////////////////
// Namespace

}

////////////////////////////////////////////////////////////////////////////////
