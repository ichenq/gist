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
//
// the core algorithm here was borrowed from:
// Twitter's `snowflake` https://github.com/twitter/snowflake

#include <stdint.h>

class SnowFlake
{
public:
    explicit SnowFlake(uint64_t worker);
    SnowFlake();
    ~SnowFlake();

    uint64_t WorkerId() const { return worker_id_; }

    bool Init(uint64_t worker_id);

    uint64_t Next();

private:
    uint64_t TickToNextMilsec(uint64_t ts);

private:
    uint64_t lastTimestamp_ = 0;    // 42 bits
    uint64_t worker_id_ = 0;        // 10 bits
    uint64_t sequence_ = 0;         // 12 bits
    uint64_t last_id_ = 0;          // 
};