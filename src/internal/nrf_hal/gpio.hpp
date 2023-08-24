/*
 * gpio.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstdbool>
#include <cstdint>
#include <cstdlib>
#include <nrf_gpio.h>
#include "types.hpp"

////////////////////////////////////////////////////////////////////////////////
// Namespace

namespace nrf_hal
{

////////////////////////////////////////////////////////////////////////////////
// GpioInputPin

template <uint32_t PIN, nrf_gpio_pin_pull_t PULL>
class GpioInputPin
{
public:
    static void begin()
    {
        nrf_gpio_cfg_input(PIN, PULL);
    }

    static void end()
    {
        nrf_gpio_cfg_input(PIN, NRF_GPIO_PIN_NOPULL);
    }

    static int read()
    {
        return nrf_gpio_pin_read(PIN) ? 1 : 0;
    }

};

////////////////////////////////////////////////////////////////////////////////
// GpioOutputPin

template <uint32_t PIN, int INITIAL_VALUE>
class GpioOutputPin
{
public:
    static void begin()
    {
        nrf_gpio_cfg_output(PIN);
        nrf_gpio_pin_write(PIN, INITIAL_VALUE != 0 ? 1 : 0);
    }

    static void end()
    {
        nrf_gpio_cfg_input(PIN, NRF_GPIO_PIN_NOPULL);
    }

    static void write(int value)
    {
        nrf_gpio_pin_write(PIN, value != 0 ? 1 : 0);
    }

};

////////////////////////////////////////////////////////////////////////////////
// Namespace

}

////////////////////////////////////////////////////////////////////////////////
