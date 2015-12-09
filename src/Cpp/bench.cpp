// Copyright (C) 2014-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include "bench.h"
#include <assert.h>
#include <time.h>
#include <limits>
#include <iostream>
 
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm")
#else
#include <uuid/uuid.h>
#endif
 
using namespace std;
 
#ifdef _WIN32
TimeCounter::TimeCounter()
{
    start_ = timeGetTime();
}
 
TimeCounter::~TimeCounter()
{
    auto elapsed = timeGetTime() - start_;
    cout << "elapsed milsec: " << elapsed << endl;
}
 
#else // GNU linux
 
inline uint64_t timespecDiff(timespec end, timespec start)
{
  if (end.tv_sec == start.tv_sec) {
    assert(end.tv_nsec >= start.tv_nsec);
    return end.tv_nsec - start.tv_nsec;
  }
  assert(end.tv_sec > start.tv_sec &&
         end.tv_sec - start.tv_sec <
         std::numeric_limits<uint64_t>::max() / 1000000000UL);
  return (end.tv_sec - start.tv_sec) * 1000000000UL
    + end.tv_nsec - start.tv_nsec;
}
 
TimeCounter::TimeCounter()
{
    int r = clock_gettime(CLOCK_REALTIME, &start_);
    assert(r == 0);
}
 
TimeCounter::~TimeCounter()
{
    timespec end;
    int r = clock_gettime(CLOCK_REALTIME, &end);
    assert(r == 0);
    auto elapsed = timespecDiff(end, start_);
    cout << "elapsed milsec: " << elapsed / 1000000.0 << endl;
}
 
#endif // _WIN32
 
UUID_T  createUUID()
{
    UUID_T uuid;
#ifdef _WIN32
    CoCreateGuid((GUID*)&uuid);
#else
    uuid_generate(uuid.data());
#endif    
    return uuid;
}

uint32_t getTickCount()
 {
#ifdef _WIN32
    return timeGetTime();
#else
    timespec end;
    int r = clock_gettime(CLOCK_REALTIME, &end);
    assert(r == 0);
    return (end.tv_sec * 1000UL) + (end.tv_nsec / 1000000UL);
#endif
 }