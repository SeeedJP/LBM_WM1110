/*
 * lr11xx_hal.cpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#include "lbm/smtc_modem_core/radio_drivers/lr11xx_driver/src/lr11xx_hal.h"
#include <string.h>
#include "internal/nrf_hal/system.hpp"
#include "internal/Wm1110Hardware.hpp"

lr11xx_hal_status_t lr11xx_hal_write( const void* context, const uint8_t* command, const uint16_t command_length,
                                      const uint8_t* data, const uint16_t data_length )
{
    if (context == nullptr) return LR11XX_HAL_STATUS_ERROR;
    if (command_length >= 1 && command == nullptr) return LR11XX_HAL_STATUS_ERROR;
    if (command_length <= 0 && command != nullptr) return LR11XX_HAL_STATUS_ERROR;
    if (data_length >= 1 && data == nullptr) return LR11XX_HAL_STATUS_ERROR;
    if (data_length <= 0 && data != nullptr) return LR11XX_HAL_STATUS_ERROR;
    if (command_length <= 0 && data_length <= 0) return LR11XX_HAL_STATUS_ERROR;

    Wm1110Hardware& hardware = *reinterpret_cast<Wm1110Hardware*>(const_cast<void*>(context));

    uint8_t tx_data[command_length + data_length];
    memcpy(tx_data, command, command_length);
    memcpy(tx_data + command_length, data, data_length);

    hardware.wakeupAndWaitForReady();
    hardware.beginTransfer();
    hardware.transfer(tx_data, nullptr, sizeof(tx_data));
    hardware.endTransfer();

    if (command_length >= 2 && command[0] == 0x01 && command[1] == 0x1b)    // LR11XX_SYSTEM_SET_SLEEP_OC
    {
        hardware.changedToSleep();

        // add a incompressible delay to prevent trying to wake the radio before it is full asleep
        nrf_hal::System::SpinDelayUs(500);
    }

    return LR11XX_HAL_STATUS_OK;
}

lr11xx_hal_status_t lr11xx_hal_read( const void* context, const uint8_t* command, const uint16_t command_length,
                                     uint8_t* data, const uint16_t data_length )
{
    if (context == nullptr) return LR11XX_HAL_STATUS_ERROR;
    if (command_length <= 0 || command == nullptr) return LR11XX_HAL_STATUS_ERROR;
    if (data_length >= 1 && data == nullptr) return LR11XX_HAL_STATUS_ERROR;
    if (data_length <= 0 && data != nullptr) return LR11XX_HAL_STATUS_ERROR;

    Wm1110Hardware& hardware = *reinterpret_cast<Wm1110Hardware*>(const_cast<void*>(context));

    hardware.wakeupAndWaitForReady();
    hardware.beginTransfer();
    hardware.transfer(command, nullptr, command_length);
    hardware.endTransfer();

    if (data_length >= 1)
    {
        uint8_t tx_data[1 + data_length];
        memset(tx_data, LR11XX_NOP, sizeof(tx_data));

        uint8_t rx_data[1 + data_length];

        hardware.wakeupAndWaitForReady();
        hardware.beginTransfer();
        hardware.transfer(tx_data, rx_data, sizeof(rx_data));
        hardware.endTransfer();

        memcpy(data, rx_data + 1, data_length);
    }

    return LR11XX_HAL_STATUS_OK;
}

lr11xx_hal_status_t lr11xx_hal_direct_read( const void* context, uint8_t* data, const uint16_t data_length )
{
    if (context == nullptr) return LR11XX_HAL_STATUS_ERROR;
    if (data_length <= 0 || data == nullptr) return LR11XX_HAL_STATUS_ERROR;

    Wm1110Hardware& hardware = *reinterpret_cast<Wm1110Hardware*>(const_cast<void*>(context));

    hardware.wakeupAndWaitForReady();
    hardware.beginTransfer();
    hardware.transfer(nullptr, data, data_length);
    hardware.endTransfer();

    return LR11XX_HAL_STATUS_OK;
}

lr11xx_hal_status_t lr11xx_hal_reset( const void* context )
{
    if (context == nullptr) return LR11XX_HAL_STATUS_ERROR;

    Wm1110Hardware& hardware = *reinterpret_cast<Wm1110Hardware*>(const_cast<void*>(context));

    hardware.reset();

    return LR11XX_HAL_STATUS_OK;
}

lr11xx_hal_status_t lr11xx_hal_wakeup( const void* context )
{
    if (context == nullptr) return LR11XX_HAL_STATUS_ERROR;

    Wm1110Hardware& hardware = *reinterpret_cast<Wm1110Hardware*>(const_cast<void*>(context));

    hardware.wakeupAndWaitForReady();

    return LR11XX_HAL_STATUS_OK;
}
