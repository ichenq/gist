// Copyright of BeyondTech, Inc. 2014-2016. All rights reserved.
//
// http://www.chaoyuehudong.com
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.


#include "stdafx.h"
#include "SnowFlake.h"
#include <cstdlib>
#include <chrono>
#include <functional>
#include "Conv.h"
#include "Net/IPAddress.h"

using namespace std;

#ifdef _WIN32
#include <windows.h>
static std::string UserLoginName()
{
    DWORD len = 128;
    char buffer[128] = {};
    ::GetUserName(buffer, &len);
    return std::string(buffer);
}
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
std::string UserLoginName()
{
    struct passwd* pwd = getpwuid(getuid());
    if (pwd != NULL)
    {
        return std::string(pwd->pw_name);
    }
    return "";
}
#endif

const uint64_t WorkerIDBits = 10;
const uint64_t SequenceBits = 12;
const uint64_t WorkerIDShift = SequenceBits;
const uint64_t TimestampShift = SequenceBits + WorkerIDBits;

// Custom epoch, (2014-11-06 00:00:00 UTC) in millisecond
const uint64_t Twepoch = 1415232000000;


// One machine should always have same worker ID
// We try hardware MAC address first, then user login name instead
static uint64_t MakeWorkerID()
{
    std::string identity;
    std::vector<std::string> list = net::GetLocalMacAddressList();
    if (!list.empty())
    {
        identity = list[0];
    }
    else
    {
        identity = UserLoginName();
    }
    if (!identity.empty())
    {
        std::hash<std::string> hasher;
        return hasher(identity) % (1 << WorkerIDBits);
    }
    BEATS_ASSERT(false, "unexpected worker id");
    return uint64_t(rand()) % (1 << WorkerIDBits);
}

inline uint64_t GetNowTimestampInMs()
{
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}

////////////////////////////////////////////////////////////////////////////////
SnowFlake::SnowFlake(uint64_t worker)
    : worker_id_(worker)
{
    Init(worker);
}

SnowFlake::SnowFlake()
{
    Init(MakeWorkerID());
}

SnowFlake::~SnowFlake()
{
}

bool SnowFlake::Init(uint64_t worker_id)
{
    if (worker_id >= (1 << WorkerIDBits))
    {
        BEATS_ASSERT(false, "snowflake worker ID {} out of range", to<std::string>(worker_id).c_str());
        return false;
    }
    worker_id_ = worker_id;
    lastTimestamp_ = GetNowTimestampInMs();
    return true;
}

uint64_t SnowFlake::Next()
{
    uint64_t ts = GetNowTimestampInMs();
    if (ts < lastTimestamp_)
    {
        throw std::runtime_error(to<std::string>("time has gone backwards, ", ts, "/", lastTimestamp_));
    }
    if (ts == lastTimestamp_)
    {
        sequence_ += 1;
        if (sequence_ == (1 << SequenceBits)) // sequence expired, tick to next millisecond
        {
            sequence_ = 0;
            ts = TickToNextMilsec(ts);
        }
    }
    else
    {
        sequence_ = 0;
    }
    lastTimestamp_ = ts;
    uint64_t id = (((ts - Twepoch) << TimestampShift) | (worker_id_ << WorkerIDShift) | sequence_);
    if (id <= last_id_)
    {
        throw std::runtime_error(to<std::string>("ID went gone backwards, ", id, "/", last_id_));
    }
    last_id_ = id;
    return id;
}

uint64_t SnowFlake::TickToNextMilsec(uint64_t ts)
{
    while (true)
    {
        uint64_t now = GetNowTimestampInMs();
        if (now > ts)
        {
            return now;
        }
    }
}
