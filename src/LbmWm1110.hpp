/*
 * LbmWm1110.hpp
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes

#include <functional>
#include <InternalFileSystem.h> // Load dependency library

////////////////////////////////////////////////////////////////////////////////
// Types

struct ralf_s;
typedef struct ralf_s ralf_t;

////////////////////////////////////////////////////////////////////////////////
// LbmWm1110

class LbmWm1110
{
private:
    static LbmWm1110* instance_;

public:
    static LbmWm1110& getInstance();

private:
    LbmWm1110();
    LbmWm1110(const LbmWm1110&) = delete;

public:
    static void attachGnssPrescan(const std::function<void(void*)>& func, void* context = nullptr);
    static void attachGnssPostscan(const std::function<void(void*)>& func, void* context = nullptr);

    static void begin();
    static ralf_t* getRadio();
    static void startWatchdog();
    static void reloadWatchdog();

};

////////////////////////////////////////////////////////////////////////////////
