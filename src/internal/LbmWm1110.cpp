/*
 * LbmWm1110.cpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

////////////////////////////////////////////////////////////////////////////////
// Includes

#include "LbmWm1110.hpp"
#include "Wm1110Hardware.hpp"

////////////////////////////////////////////////////////////////////////////////
// Variables

LbmWm1110* LbmWm1110::instance_ = nullptr;

////////////////////////////////////////////////////////////////////////////////
// LbmWm1110

LbmWm1110& LbmWm1110::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new LbmWm1110();
    }

    return *instance_;
}

LbmWm1110::LbmWm1110()
{
}

void LbmWm1110::attachGnssPrescan(const std::function<void(void*)>& func, void* context)
{
    Wm1110Hardware::getInstance().attachGnssPrescan(func, context);
}

void LbmWm1110::attachGnssPostscan(const std::function<void(void*)>& func, void* context)
{
    Wm1110Hardware::getInstance().attachGnssPostscan(func, context);
}

void LbmWm1110::begin()
{
    Wm1110Hardware::getInstance().begin();
}

ralf_t* LbmWm1110::getRadio()
{
    return &Wm1110Hardware::getInstance().radio;
}

void LbmWm1110::startWatchdog()
{
    Wm1110Hardware::getInstance().wdt.begin();
}

void LbmWm1110::reloadWatchdog()
{
    Wm1110Hardware::getInstance().wdt.reload();
}

////////////////////////////////////////////////////////////////////////////////
