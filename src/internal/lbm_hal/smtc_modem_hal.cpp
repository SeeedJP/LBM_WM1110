/*
 * smtc_modem_hal.cpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#include "lbm/smtc_modem_hal/smtc_modem_hal.h"
#include <cstdlib>
#include <cstring>
#include <type_traits>
#include "internal/nrf_hal/system.hpp"
#include "internal/nrf_hal/trace.hpp"
#include "internal/Wm1110Hardware.hpp"

static constexpr uintptr_t FLASH_APPLICATION_END_ADDR = 0xed000;    // Look at nrf52840_s140_v6.ld

static const uint32_t FlashLbmConextPages[MODEM_CONTEXT_TYPE_SIZE] =
{
    FLASH_APPLICATION_END_ADDR / Wm1110Hardware::getInstance().flash.PAGE_SIZE - 1, // CONTEXT_MODEM
    FLASH_APPLICATION_END_ADDR / Wm1110Hardware::getInstance().flash.PAGE_SIZE - 2, // CONTEXT_LR1MAC
    FLASH_APPLICATION_END_ADDR / Wm1110Hardware::getInstance().flash.PAGE_SIZE - 3, // CONTEXT_DEVNONCE
    FLASH_APPLICATION_END_ADDR / Wm1110Hardware::getInstance().flash.PAGE_SIZE - 4, // CONTEXT_SECURE_ELEMENT
};

static uint8_t CrashLog[CRASH_LOG_SIZE];
static volatile bool CrashLogAvailable;

/* ------------ Reset management ------------*/

void smtc_modem_hal_reset_mcu( void )
{
    nrf_hal::System::reset();
}

/* ------------ Watchdog management ------------*/

void smtc_modem_hal_reload_wdog( void )
{
    // No need to implement since this function is not called from SWL2001
}

/* ------------ Time management ------------*/

uint32_t smtc_modem_hal_get_time_in_s( void )
{
    return Wm1110Hardware::getInstance().rtcTimer.getElapsedSeconds();
}

uint32_t smtc_modem_hal_get_compensated_time_in_s( void )
{
    return Wm1110Hardware::getInstance().rtcTimer.getElapsedSeconds();
}

int32_t smtc_modem_hal_get_time_compensation_in_s( void )
{
    return 0;
}

uint32_t smtc_modem_hal_get_time_in_ms( void )
{
    return Wm1110Hardware::getInstance().rtcTimer.getElapsedMilliseconds();
}

uint32_t smtc_modem_hal_get_time_in_100us( void )
{
    return Wm1110Hardware::getInstance().rtcTimer.getElapsed100Microseconds();
}

uint32_t smtc_modem_hal_get_radio_irq_timestamp_in_100us( void )
{
    return Wm1110Hardware::getInstance().rtcTimer.getElapsed100Microseconds();
}

/* ------------ Timer management ------------*/

void smtc_modem_hal_start_timer( const uint32_t milliseconds, void ( *callback )( void* context ), void* context )
{
    if (milliseconds <= 0) abort();
    if (callback == nullptr) abort();

    Wm1110Hardware::getInstance().timerOneshot.start(milliseconds, callback, context);
}

void smtc_modem_hal_stop_timer( void )
{
    Wm1110Hardware::getInstance().timerOneshot.stop();
}

/* ------------ IRQ management ------------*/

void smtc_modem_hal_disable_modem_irq( void )
{
    Wm1110Hardware& hardware = Wm1110Hardware::getInstance();

    hardware.timerOneshot.disableIRQ();
    hardware.irqEvent.disableIRQ();
}

void smtc_modem_hal_enable_modem_irq( void )
{
    Wm1110Hardware& hardware = Wm1110Hardware::getInstance();

    hardware.timerOneshot.enableIRQ();
    hardware.irqEvent.enableIRQ();
}

/* ------------ Context saving management ------------*/

void smtc_modem_hal_context_restore( const modem_context_type_t ctx_type, uint8_t* buffer, const uint32_t size )
{
    if (std::extent<decltype(FlashLbmConextPages)>::value <= ctx_type) abort();
    if (buffer == nullptr) abort();
    if (size <= 0 || Wm1110Hardware::getInstance().flash.PAGE_SIZE < size) abort();

    const uint32_t page = FlashLbmConextPages[ctx_type];

    Wm1110Hardware::getInstance().flash.read(page, buffer, size);
}

void smtc_modem_hal_context_store( const modem_context_type_t ctx_type, const uint8_t* buffer, const uint32_t size )
{
    if (std::extent<decltype(FlashLbmConextPages)>::value <= ctx_type) abort();
    if (buffer == nullptr) abort();
    if (size <= 0 || Wm1110Hardware::getInstance().flash.PAGE_SIZE < size) abort();

    const uint32_t page = FlashLbmConextPages[ctx_type];

    Wm1110Hardware::getInstance().flash.write(page, buffer, size);
}

/* ------------ Crashlog management ------------*/

void smtc_modem_hal_store_crashlog( uint8_t crashlog[CRASH_LOG_SIZE] )
{
    memcpy(CrashLog, crashlog, CRASH_LOG_SIZE);
}

void smtc_modem_hal_restore_crashlog( uint8_t crashlog[CRASH_LOG_SIZE] )
{
    memcpy(crashlog, CrashLog, CRASH_LOG_SIZE);
}

void smtc_modem_hal_set_crashlog_status( bool available )
{
    CrashLogAvailable = available;
}

bool smtc_modem_hal_get_crashlog_status( void )
{
    return CrashLogAvailable;
}

/* ------------ assert management ------------*/

void smtc_modem_hal_assert_fail( uint8_t* func, uint32_t line )
{
    smtc_modem_hal_store_crashlog(func);
    smtc_modem_hal_set_crashlog_status(true);
    smtc_modem_hal_print_trace("\x1B[0;31m" "crash log :%s:%u\n" "\x1B[0m", func, line);
    smtc_modem_hal_reset_mcu();
}

/* ------------ Random management ------------*/

uint32_t smtc_modem_hal_get_random_nb( void )
{
    return Wm1110Hardware::getInstance().rng.read();
}

uint32_t smtc_modem_hal_get_random_nb_in_range( const uint32_t val_1, const uint32_t val_2 )
{
    uint32_t minValue;
    uint32_t maxValue;
    if (val_1 < val_2)
    {
        minValue = val_1;
        maxValue = val_2;
    }
    else if (val_2 < val_1)
    {
        minValue = val_2;
        maxValue = val_1;
    }
    else
    {
        return val_1;
    }

    if (minValue == 0 && maxValue == UINT32_MAX)
    {
        return Wm1110Hardware::getInstance().rng.read();
    }
    else
    {
        return Wm1110Hardware::getInstance().rng.read() % (maxValue - minValue + 1) + minValue;
    }
}

int32_t smtc_modem_hal_get_signed_random_nb_in_range( const int32_t val_1, const int32_t val_2 )
{
    int32_t minValue;
    int32_t maxValue;
    if (val_1 < val_2)
    {
        minValue = val_1;
        maxValue = val_2;
    }
    else if (val_2 < val_1)
    {
        minValue = val_2;
        maxValue = val_1;
    }
    else
    {
        return val_1;
    }

    if (minValue == INT32_MIN && maxValue == INT32_MAX)
    {
        return Wm1110Hardware::getInstance().rng.read() + INT32_MIN;
    }
    else
    {
        return Wm1110Hardware::getInstance().rng.read() % (maxValue - minValue + 1) + minValue;
    }
}

/* ------------ Radio env management ------------*/

void smtc_modem_hal_irq_config_radio_irq( void ( *callback )( void* context ), void* context )
{
    if (callback == nullptr) abort();

    Wm1110Hardware::getInstance().irqEvent.begin(RISING, callback, context);
}

void smtc_modem_hal_radio_irq_clear_pending( void )
{
    Wm1110Hardware::getInstance().irqEvent.clearIRQ();
}

void smtc_modem_hal_start_radio_tcxo( void )
{
}

void smtc_modem_hal_stop_radio_tcxo( void )
{
}

uint32_t smtc_modem_hal_get_radio_tcxo_startup_delay_ms( void )
{
    return 30;
}

/* ------------ Environment management ------------*/

uint8_t smtc_modem_hal_get_battery_level( void )
{
    return 254;
}

int8_t smtc_modem_hal_get_temperature( void )
{
    return 20;
}

uint8_t smtc_modem_hal_get_voltage( void )
{
    return 254;
}

int8_t smtc_modem_hal_get_board_delay_ms( void )
{
    return 1;
}

/* ------------ Trace management ------------*/

void smtc_modem_hal_print_trace( const char* fmt, ... )
{
    va_list args;

    va_start(args, fmt);
    nrf_hal::Trace::vprintf(fmt, args);
    va_end(args);
}
