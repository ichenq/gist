// Copyright (C) 2014-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#pragma once
 
#include <time.h>
#include <stdint.h>
#include <array>


typedef std::array<uint8_t, 16>     UUID_T;



#ifdef _MSC_VER
#pragma optimize("", off)
template <class T>
void doNotOptimizeAway(T&& datum) {
    datum = datum;
}
#pragma optimize("", on)
#else
template <class T>
void doNotOptimizeAway(T&& datum) {
    asm volatile("" : "+r" (datum));
}
#endif
 
class TimeCounter
{
public:
    TimeCounter();
    ~TimeCounter();
    
private:
#ifdef _WIN32
    uint32_t    start_;
#else
    timespec    start_;
#endif
};
 
UUID_T  createUUID();