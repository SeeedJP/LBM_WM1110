/*
 * flash.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <nrfx_nvmc.h>
#include "system.hpp"
#include <flash/flash_nrf5x.h>  // Adafruit Internal File System on Bluefruit nRF52

////////////////////////////////////////////////////////////////////////////////
// Namespace

namespace nrf_hal
{

////////////////////////////////////////////////////////////////////////////////
// Flash

class Flash
{
private:
    static constexpr size_t CHUNK_LEN = 16 / sizeof(uint32_t);  // 16bytes / sizeof(uint32_t)

public:
    static constexpr int PAGE_SIZE = 4096;                      // nrfx_nvmc_flash_page_size_get()

public:
    static void write(uint32_t page, const void* buffer, size_t size)
    {
        if (nrfx_nvmc_flash_page_size_get() != PAGE_SIZE) abort();
        if (page >= nrfx_nvmc_flash_page_count_get()) abort();
        if (buffer == nullptr) abort();
        if (size <= 0 || PAGE_SIZE < size) abort();

        const uintptr_t ptr = page * PAGE_SIZE;

#ifdef SOFTDEVICE_PRESENT
#ifndef ARDUINO_NRF52_ADAFRUIT
#error Only supports Adafruit_nRF52_Arduino
#endif
        if (System::isSoftDeviceEnabled())
        {
            if (!flash_nrf5x_erase(page * PAGE_SIZE)) abort();

            if (static_cast<size_t>(flash_nrf5x_write(ptr, buffer, size)) != size) abort();

            flash_nrf5x_flush();
        }
        else
#endif
        {
            if (nrfx_nvmc_page_erase(page * PAGE_SIZE) != NRFX_SUCCESS) abort();

            uint32_t chunk[CHUNK_LEN] __ALIGNED(4);
            for (uint32_t i = 0; i < size / sizeof(chunk); ++i)
            {
                memcpy(chunk, reinterpret_cast<const uint8_t*>(buffer) + i * sizeof(chunk), sizeof(chunk));
                nrfx_nvmc_words_write(ptr + i * sizeof(chunk), chunk, std::extent<decltype(chunk)>::value);
            }
            const size_t restSize = size % sizeof(chunk);
            if (restSize >= 1)
            {
                if (sizeof(chunk) - restSize >= 1)
                {
                    memset(reinterpret_cast<uint8_t*>(chunk) + restSize, 0xff, sizeof(chunk) - restSize);
                }
                memcpy(chunk, reinterpret_cast<const uint8_t*>(buffer) + size / sizeof(chunk) * sizeof(chunk), restSize);
                nrfx_nvmc_words_write(ptr + size / sizeof(chunk) * sizeof(chunk), chunk, std::extent<decltype(chunk)>::value);
            }

            while (!nrfx_nvmc_write_done_check()){} // Seeed: Spin
        }
    }

    static void read(uint32_t page, void* buffer, size_t size)
    {
        if (nrfx_nvmc_flash_page_size_get() != PAGE_SIZE) abort();
        if (page >= nrfx_nvmc_flash_page_count_get()) abort();
        if (buffer == nullptr) abort();
        if (size <= 0 || PAGE_SIZE < size) abort();

        const uintptr_t ptr = page * PAGE_SIZE;

        memcpy(buffer, reinterpret_cast<const void*>(ptr), size);
    }

};

////////////////////////////////////////////////////////////////////////////////
// Namespace

}

////////////////////////////////////////////////////////////////////////////////
