/*
 * spim.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <nrfx_spim.h>

////////////////////////////////////////////////////////////////////////////////
// Namespace

namespace nrf_hal
{

////////////////////////////////////////////////////////////////////////////////
// Spim

template <int ID, uint8_t SCLK_PIN, uint8_t MOSI_PIN, uint8_t MISO_PIN, nrf_spim_frequency_t FREQUENCY>
class Spim
{
private:
    static nrfx_spim_t spim_;

public:
    Spim()
    {
#if NRFX_CHECK(NRFX_SPIM0_ENABLED)
        if (ID == 0) { spim_ = NRFX_SPIM_INSTANCE(0); return; }
#endif
#if NRFX_CHECK(NRFX_SPIM1_ENABLED)
        if (ID == 1) { spim_ = NRFX_SPIM_INSTANCE(1); return; }
#endif
#if NRFX_CHECK(NRFX_SPIM2_ENABLED)
        if (ID == 2) { spim_ = NRFX_SPIM_INSTANCE(2); return; }
#endif
#if NRFX_CHECK(NRFX_SPIM3_ENABLED)
        if (ID == 3) { spim_ = NRFX_SPIM_INSTANCE(3); return; }
#endif
#if NRFX_CHECK(NRFX_SPIM4_ENABLED)
        if (ID == 4) { spim_ = NRFX_SPIM_INSTANCE(4); return; }
#endif
        abort();
    }

    static void begin()
    {
        nrfx_spim_config_t config = NRFX_SPIM_DEFAULT_CONFIG(SCLK_PIN, MOSI_PIN, MISO_PIN, NRFX_SPIM_PIN_NOT_USED);
        config.frequency = FREQUENCY;
        
        if (nrfx_spim_init(&spim_, &config, nullptr, nullptr) != NRFX_SUCCESS) abort();
    }

    static void transfer(const uint8_t* txData, uint8_t* rxData, size_t length)
    {
        if (txData == nullptr && rxData == nullptr) abort();
        if (length <= 0) abort();

        const size_t txLength = txData != nullptr ? length : 0;
        const size_t rxLength = rxData != nullptr ? length : 0;

        const nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(txData, txLength, rxData, rxLength);
        if (nrfx_spim_xfer(&spim_, &xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER) != NRFX_SUCCESS) abort();
    }

};

template <int ID, uint8_t SCLK_PIN, uint8_t MOSI_PIN, uint8_t MISO_PIN, nrf_spim_frequency_t FREQUENCY>
nrfx_spim_t Spim<ID, SCLK_PIN, MOSI_PIN, MISO_PIN, FREQUENCY>::spim_;

////////////////////////////////////////////////////////////////////////////////
// Namespace

}

////////////////////////////////////////////////////////////////////////////////
