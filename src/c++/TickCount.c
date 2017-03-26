// Copyright (C) 2014-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <stdint.h>
#include <assert.h>
#include <time.h>

#define CHECK   assert

#if defined(_MSC_VER)
#include <windows.h>
uint64_t getPerformanceFreqency()
{
    static LARGE_INTEGER freq;
    if (freq.QuadPart == 0)
    {
        CHECK(QueryPerformanceFrequency(&freq));
    }
    return freq.QuadPart;
}

uint64_t getNowTickCount()
{
    LARGE_INTEGER now;
    CHECK(QueryPerformanceCounter(&now));
    return (now.QuadPart * 1000000000UL) / getPerformanceFreqency();
}

#elif defined(__GNUC__)

uint64_t getNowTickCount()
{
    timespec ts;
    CHECK(clock_gettime(CLOCK_REALTIME, &ts) == 0);
    return (ts.tv_sec * 1000000000UL) + ts.tv_nsec;
}

#endif