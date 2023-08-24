/*
 * timer.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <stdint.h>
#include <cstdlib>
#include <nrfx_timer.h>
#include "types.hpp"

////////////////////////////////////////////////////////////////////////////////
// Namespace

namespace nrf_hal
{

////////////////////////////////////////////////////////////////////////////////
// TimerOneshot

template <int ID>
class TimerOneshot
{
private:
    static nrfx_timer_t timer_;
    static IsrAndContext isr_;

private:
    static void timerIsr(nrf_timer_event_t event_type, void* p_context)
    {
        if (event_type != NRF_TIMER_EVENT_COMPARE0) abort();

        if (isr_.Isr == nullptr) abort();

        nrfx_timer_disable(&timer_);

        isr_.Isr(isr_.Context);
    }

public:
    TimerOneshot()
    {
#if NRFX_CHECK(NRFX_TIMER0_ENABLED)
        if (ID == 0) { timer_ = NRFX_TIMER_INSTANCE(0); return; }
#endif
#if NRFX_CHECK(NRFX_TIMER1_ENABLED)
        if (ID == 1) { timer_ = NRFX_TIMER_INSTANCE(1); return; }
#endif
#if NRFX_CHECK(NRFX_TIMER2_ENABLED)
        if (ID == 2) { timer_ = NRFX_TIMER_INSTANCE(2); return; }
#endif
#if NRFX_CHECK(NRFX_TIMER3_ENABLED)
        if (ID == 3) { timer_ = NRFX_TIMER_INSTANCE(3); return; }
#endif
#if NRFX_CHECK(NRFX_TIMER4_ENABLED)
        if (ID == 4) { timer_ = NRFX_TIMER_INSTANCE(4); return; }
#endif
        abort();
    }

    static void begin()
    {
        nrfx_timer_config_t config = NRFX_TIMER_DEFAULT_CONFIG;
        config.frequency = NRF_TIMER_FREQ_125kHz;
        config.bit_width = NRF_TIMER_BIT_WIDTH_32;

        if (nrfx_timer_init(&timer_, &config, timerIsr) != NRFX_SUCCESS) abort();
    }

    static void start(uint32_t milliseconds, void (*callback)(void* context), void* context)
    {
        if (milliseconds <= 0) abort();
        if (callback == nullptr) abort();

        if (nrfx_timer_is_enabled(&timer_)) abort();

        isr_ = { callback, context };

        nrfx_timer_extended_compare(&timer_, NRF_TIMER_CC_CHANNEL0, nrfx_timer_ms_to_ticks(&timer_, milliseconds), NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
        nrfx_timer_clear(&timer_);

        nrfx_timer_enable(&timer_);
    }

    static void stop()
    {
        nrfx_timer_disable(&timer_);
    }

    static void enableIRQ()
    {
        nrf_timer_int_enable(timer_.p_reg, nrf_timer_compare_int_get(NRF_TIMER_CC_CHANNEL0));
    }

    static void disableIRQ()
    {
        nrf_timer_int_disable(timer_.p_reg, nrf_timer_compare_int_get(NRF_TIMER_CC_CHANNEL0));
    }

};

template <int ID>
nrfx_timer_t TimerOneshot<ID>::timer_;

template <int ID>
IsrAndContext TimerOneshot<ID>::isr_;

////////////////////////////////////////////////////////////////////////////////
// Namespace

}

////////////////////////////////////////////////////////////////////////////////
