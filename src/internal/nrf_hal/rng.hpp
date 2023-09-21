/*
 * rng.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <nrf_soc.h>
#include <nrfx_rng.h>
#include "system.hpp"

////////////////////////////////////////////////////////////////////////////////
// Namespace

namespace nrf_hal
{

////////////////////////////////////////////////////////////////////////////////
// Rng

class Rng
{
private:
    static volatile uint8_t rngValue_[4];
    static volatile size_t rngValueCount_;

private:
    static void rngIsr(uint8_t rng_data)
    {
        if (sizeof(rngValue_) <= rngValueCount_) abort();

        rngValue_[rngValueCount_] = rng_data;
        ++rngValueCount_;
        if (rngValueCount_ >= sizeof(rngValue_)) nrfx_rng_stop();
    }

public:
    static void begin(uint8_t interruptPriority = NRFX_GPIOTE_DEFAULT_CONFIG_IRQ_PRIORITY)
    {
#ifdef SOFTDEVICE_PRESENT
        if (System::isSoftDeviceEnabled())
        {
            uint8_t capacity;
            if (sd_rand_application_pool_capacity_get(&capacity) != NRF_SUCCESS) abort();
            if (capacity < 4) abort();
        }
        else
#endif
        {
            nrfx_rng_config_t rng_config = NRFX_RNG_DEFAULT_CONFIG;

            if (nrfx_rng_init(&rng_config, rngIsr) != NRFX_SUCCESS) abort();

            rngValueCount_ = 0;
            nrfx_rng_start();
        }
    }

    static uint32_t read()
    {
#ifdef SOFTDEVICE_PRESENT
        if (System::isSoftDeviceEnabled())
        {
            uint8_t available;
            do
            {
                if (sd_rand_application_bytes_available_get(&available) != NRF_SUCCESS) abort();
            }
            while (available < 4);  // Seeed: Spin

            uint32_t value;
            if (sd_rand_application_vector_get(reinterpret_cast<uint8_t*>(&value), sizeof(value)) != NRF_SUCCESS) abort();

            return value;
        }
        else
#endif
        {
            while (rngValueCount_ < sizeof(rngValue_)){}    // Seeed: Spin

            const uint32_t value = rngValue_[3] << 24 |
                                   rngValue_[2] << 16 |
                                   rngValue_[1] << 8  |
                                   rngValue_[0];

            rngValueCount_ = 0;
            nrfx_rng_start();

            return value;
        }
    }

};

////////////////////////////////////////////////////////////////////////////////
// Namespace

}

////////////////////////////////////////////////////////////////////////////////
