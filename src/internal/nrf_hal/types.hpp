/*
 * types.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Namespace

namespace nrf_hal
{

////////////////////////////////////////////////////////////////////////////////
// IsrAndContext

struct IsrAndContext
{
    void (*Isr)(void*);
    void* Context;

    IsrAndContext() :
        Isr{ nullptr },
        Context{ nullptr }
    {
    }

    IsrAndContext(void (*isr)(void*), void* context) :
        Isr{ isr },
        Context{ context }
    {
    }

};

////////////////////////////////////////////////////////////////////////////////
// Namespace

}

////////////////////////////////////////////////////////////////////////////////
