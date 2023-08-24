/*
 * Wm1110Hardware.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <functional>
#include <tuple>
#include "internal/nrf_hal/gpio.hpp"
#include "internal/nrf_hal/spim.hpp"
#include "internal/nrf_hal/flash.hpp"
#include "internal/nrf_hal/rtc.hpp"
#include "internal/nrf_hal/gpiote.hpp"
#include "internal/nrf_hal/rng.hpp"
#include "internal/nrf_hal/timer.hpp"
#include "internal/nrf_hal/wdt.hpp"
#include "lbm/smtc_modem_core/smtc_ralf/src/ralf.h"

////////////////////////////////////////////////////////////////////////////////
// Wm1110Hardware

class Wm1110Hardware
{
private:
    enum class RadioMode
    {
        SLEEP,
        AWAKE,
    };

private:
    static constexpr int LR1110_SPI_NSS_PIN     = 44;   // P1.12
    static constexpr int LR1110_SPI_SCK_PIN     = 45;   // P1.13
    static constexpr int LR1110_SPI_MOSI_PIN    = 46;   // P1.14
    static constexpr int LR1110_SPI_MISO_PIN    = 47;   // P1.15
    static constexpr int LR1110_BUSY_PIN        = 43;   // P1.11
    static constexpr int LR1110_IRQ_PIN         = PIN_LR1110_IRQ;
    static constexpr int LR1110_NRESET_PIN      = 42;   // P1.10

    static constexpr int SPIM_ID            = 3;
    static constexpr int RTC_TIMER_ID       = 2;
    static constexpr int TIMER_ONESHOT_ID   = 2;
    static constexpr int WDT_ID             = 0;

private:
    static Wm1110Hardware* instance_;

    std::tuple<std::function<void(void*)>, void*> gnssPrescan_;
    std::tuple<std::function<void(void*)>, void*> gnssPostscan_;

    nrf_hal::GpioOutputPin<LR1110_NRESET_PIN, 0> reset_;
    nrf_hal::GpioInputPin<LR1110_BUSY_PIN, NRF_GPIO_PIN_NOPULL> busy_;
    nrf_hal::GpioInputPin<LR1110_IRQ_PIN, NRF_GPIO_PIN_PULLDOWN> irq_;
    nrf_hal::GpioOutputPin<LR1110_SPI_NSS_PIN, 1> ss_;
    nrf_hal::Spim<SPIM_ID, LR1110_SPI_SCK_PIN, LR1110_SPI_MOSI_PIN, LR1110_SPI_MISO_PIN, NRF_SPIM_FREQ_16M> spim_;
    RadioMode radioMode_;

public:
    nrf_hal::Flash flash;
    nrf_hal::RtcTimer<RTC_TIMER_ID> rtcTimer;
    nrf_hal::GpioteEventInterrupt<LR1110_IRQ_PIN> irqEvent;
    nrf_hal::Rng rng;
    nrf_hal::TimerOneshot<TIMER_ONESHOT_ID> timerOneshot;
    nrf_hal::Wdt<WDT_ID> wdt;

    ralf_t radio;

public:
    static Wm1110Hardware& getInstance();

private:
    Wm1110Hardware();
    Wm1110Hardware(const Wm1110Hardware&) = delete;

public:
    void attachGnssPrescan(const std::function<void(void*)>& func, void* context);
    void invokeGnssPrescan();
    void attachGnssPostscan(const std::function<void(void*)>& func, void* context);
    void invokeGnssPostscan();

    void begin();
    void reset();
    void beginTransfer();
    void endTransfer();
    void transfer(const uint8_t* txData, uint8_t* rxData, size_t length);
    void changedToSleep();
    void wakeupAndWaitForReady();

};

////////////////////////////////////////////////////////////////////////////////
