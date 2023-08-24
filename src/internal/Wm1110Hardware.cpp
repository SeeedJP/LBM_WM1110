/*
 * Wm1110Hardware.cpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

////////////////////////////////////////////////////////////////////////////////
// Includes

#include "Wm1110Hardware.hpp"
#include "internal/nrf_hal/system.hpp"
#include "lbm/smtc_modem_core/smtc_ralf/src/ralf_lr11xx.h"

////////////////////////////////////////////////////////////////////////////////
// Variables

Wm1110Hardware* Wm1110Hardware::instance_ = nullptr;

////////////////////////////////////////////////////////////////////////////////
// Wm1110Hardware

Wm1110Hardware& Wm1110Hardware::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new Wm1110Hardware();
    }

    return *instance_;
}

Wm1110Hardware::Wm1110Hardware()
{
    radio = RALF_LR11XX_INSTANTIATE(this);
}

void Wm1110Hardware::attachGnssPrescan(const std::function<void(void*)>& func, void* context)
{
    gnssPrescan_ = { func, context };
}

void Wm1110Hardware::invokeGnssPrescan()
{
    const std::function<void(void*)>& func = std::get<0>(gnssPrescan_);
    void* context = std::get<1>(gnssPrescan_);

    if (func) func(context);
}

void Wm1110Hardware::attachGnssPostscan(const std::function<void(void*)>& func, void* context)
{
    gnssPostscan_ = { func, context };
}

void Wm1110Hardware::invokeGnssPostscan()
{
    const std::function<void(void*)>& func = std::get<0>(gnssPostscan_);
    void* context = std::get<1>(gnssPostscan_);

    if (func) func(context);
}

void Wm1110Hardware::begin()
{
    nrf_hal::System::startHfclk();

    reset_.begin();
    busy_.begin();
    irq_.begin();
    ss_.begin();
    spim_.begin();

    rtcTimer.begin();
    rng.begin();
    timerOneshot.begin();

    radioMode_ = RadioMode::AWAKE;
}

void Wm1110Hardware::reset()
{
    ss_.write(1);

    // Putting the NRESET signal to low for at least 100 μs restarts the LR1110. / LR1110 Transceiver User Manual
    reset_.write(0);
    nrf_hal::System::SpinDelayUs(200);
    reset_.write(1);
    nrf_hal::System::SpinDelayUs(200);

    // At the end of the startup sequence, the device is set in Standby RC mode, the BUSY signal goes low and the device accepts commands. / LR1110 Transceiver User Manual
    while (busy_.read()) nrf_hal::System::delayMs(1);

    radioMode_ = RadioMode::AWAKE;
}

void Wm1110Hardware::beginTransfer()
{
    ss_.write(0);
}

void Wm1110Hardware::endTransfer()
{
    ss_.write(1);
}

void Wm1110Hardware::transfer(const uint8_t* txData, uint8_t* rxData, size_t length)
{
    spim_.transfer(txData, rxData, length);
}

void Wm1110Hardware::changedToSleep()
{
    radioMode_ = RadioMode::SLEEP;
}

void Wm1110Hardware::wakeupAndWaitForReady()
{
    if (radioMode_ == RadioMode::SLEEP)
    {
        beginTransfer();
        endTransfer();

        radioMode_ = RadioMode::AWAKE;
    }

    while (busy_.read()){}  // Seeed: Spin
}

////////////////////////////////////////////////////////////////////////////////
