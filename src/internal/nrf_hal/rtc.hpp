/*
 * rtc.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdint>
#include <cstdlib>
#include <nrfx_rtc.h>

////////////////////////////////////////////////////////////////////////////////
// Namespace

namespace nrf_hal
{

////////////////////////////////////////////////////////////////////////////////
// RtcTimer

template <int ID>
class RtcTimer
{
private:
    static constexpr int FREQUENCY = 32768;
    static constexpr uint64_t RTC_MAX_COUNTS = 0x1000000;   // nrfx_rtc_max_ticks_get()

private:
    static nrfx_rtc_t rtc_;
    static volatile uint64_t cumulativeCounts_;

private:
    static void rtcIsr(nrfx_rtc_int_type_t int_type)
    {
        if (int_type == NRFX_RTC_INT_OVERFLOW)
        {
            cumulativeCounts_ += RTC_MAX_COUNTS;
        }
    }

    static uint64_t getElapsedCounts()
    {
        uint64_t counts1st = cumulativeCounts_ + nrfx_rtc_counter_get(&rtc_);
        uint64_t counts2nd;

        do
        {
            counts2nd = counts1st;
            counts1st = cumulativeCounts_ + nrfx_rtc_counter_get(&rtc_);
        }
        while (counts1st != counts2nd);

        return counts1st;
    }

public:
    RtcTimer()
    {
#if NRFX_CHECK(NRFX_RTC0_ENABLED)
        if (ID == 0) { rtc_ = NRFX_RTC_INSTANCE(0); return; }
#endif
#if NRFX_CHECK(NRFX_RTC1_ENABLED)
        if (ID == 1) { rtc_ = NRFX_RTC_INSTANCE(1); return; }
#endif
#if NRFX_CHECK(NRFX_RTC2_ENABLED)
        if (ID == 2) { rtc_ = NRFX_RTC_INSTANCE(2); return; }
#endif
        abort();
    }

    static void begin()
    {
        nrfx_rtc_config_t config = NRFX_RTC_DEFAULT_CONFIG;
        config.prescaler = RTC_FREQ_TO_PRESCALER(FREQUENCY);

        if (nrfx_rtc_init(&rtc_, &config, rtcIsr) != NRFX_SUCCESS) abort();
        nrfx_rtc_overflow_enable(&rtc_, true);
        nrfx_rtc_enable(&rtc_);
    }

    static uint32_t getElapsedSeconds()
    {
        return getElapsedCounts() / FREQUENCY;
    }

    static uint32_t getElapsedMilliseconds()
    {
        return getElapsedCounts() * 1000 / FREQUENCY;
    }

    static uint32_t getElapsed100Microseconds()
    {
        return getElapsedCounts() * 10000 / FREQUENCY;
    }

};

template <int ID>
nrfx_rtc_t RtcTimer<ID>::rtc_;

template <int ID>
volatile uint64_t RtcTimer<ID>::cumulativeCounts_ = 0;

////////////////////////////////////////////////////////////////////////////////
// Namespace

}

////////////////////////////////////////////////////////////////////////////////
