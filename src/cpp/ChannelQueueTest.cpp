// Copyright of BeyondTech, Inc. 2014-2016. All rights reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form 
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially 
// exploit the content. Nor may you transmit it or store it in any other website or 
// other form of electronic retrieval system.

#include "stdafx.h"
#include <thread>
#include <memory>
#include <sstream>
#include <chrono>
#include <set>

#ifdef _USE_GTEST
#include <gtest/gtest.h>
#include "ChannelQueue.h"

using namespace std;

typedef std::shared_ptr<std::thread> thread_ptr;

TEST(ChannelQueue, SimpleQueue)
{
    int capacity = 100;
    ChannelQueue<int> que(capacity);
    EXPECT_EQ(que.Size(), 0);
    EXPECT_EQ(que.Capacity(), capacity);
    EXPECT_FALSE(que.IsFull());
    EXPECT_TRUE(que.IsEmpty());
    int x = 0, y = 10;
    auto r = que.TryPop(&x);
    EXPECT_EQ(r, APR_EAGAIN);

    r = que.Push(y);
    EXPECT_EQ(r, APR_SUCCESS);
    EXPECT_FALSE(que.IsEmpty());

    r = que.Pop(&x);
    EXPECT_EQ(r, APR_SUCCESS);
    EXPECT_EQ(x, y);

    for (int i = 0; i < capacity; i++)
    {
        r = que.Push(i);
        EXPECT_EQ(r, APR_SUCCESS);
    }
    EXPECT_TRUE(que.IsFull());
    r = que.TryPush(1);
    EXPECT_EQ(r, APR_EAGAIN);
}

static std::mutex g_mtx;
static std::set<int> g_consumed;


auto Consumer = [&](ChannelQueue<int>* que){
    auto tid = std::this_thread::get_id();
    int count = 0;
    while (true)
    {
        int item;
        auto r = que->Pop(&item);
        if (r == APR_SUCCESS)
        {
            std::lock_guard<std::mutex> guard(g_mtx);
            EXPECT_EQ(g_consumed.count(item), 0);
            g_consumed.insert(item);
            //std::stringstream strm;
            //strm << "consumer[" << tid << "]: " << item;
            //cout << strm.str() << endl;
            count++;
        }
        else
        {
            std::stringstream strm;
            strm << "consumer[" << tid << "] exit " << r << ", consumed: " << count;
            cout << strm.str() << endl;
            break;
        }
    }
};

TEST(ChannelQueue, MultiConsumer)
{
    int capacity = 1024;
    ChannelQueue<int> que(capacity);
    std::vector<thread_ptr> workers;
    for (int i = 1; i <= 4; i++)
    {
        workers.push_back(std::make_shared<std::thread>(std::bind(Consumer, &que)));
    }
    for (int i = 1; i <= capacity; i++)
    {
        que.Push(i);
    }
    printf("produced %d item\n", capacity);
    while (!que.IsEmpty())
        ;
    que.Terminate();
    for (int i = 0; i < 4; i++)
    {
        workers[i]->join();
    }
    EXPECT_EQ(g_consumed.size(), capacity);
}

auto Producer = [&](int min, int max, ChannelQueue<int>* que){
    int count = 0;
    auto tid = std::this_thread::get_id();
    for (int i = min; i < max; i++)
    {
        auto r = que->Push(i);
        if (r == APR_SUCCESS)
        {
            count++;
        }
        else
        {
            std::stringstream strm;
            strm << "producer[" << tid << "] exit, produced: " << count;
            cout << strm.str() << endl;
            break;
        }
    }
    std::stringstream strm;
    strm << "producer[" << tid << "] exit, produced: " << count;
    cout << strm.str() << endl;
};


TEST(ChannelQueue, MultiProducer)
{
    int capacity = 4000;
    ChannelQueue<int> que(capacity);
    std::vector<thread_ptr> workers;
    for (int i = 0; i < 4; i++)
    {
        int avg = capacity / 4;
        int min = i * avg;
        int max = min + avg;
        workers.push_back(std::make_shared<std::thread>(std::bind(Producer, min, max, &que)));
    }
    int count = 0;
    std::set<int> consumed;
    while (true)
    {
        int item;
        auto r = que.TryPop(&item);
        if (r == APR_SUCCESS)
        {
            EXPECT_EQ(consumed.count(item), 0);
            consumed.insert(item);
            count++;
        }
        else
        {
            printf("consumer exit, %d, consumed: %d\n", r, count);
            break;
        }
    }
    EXPECT_EQ(consumed.size(), capacity);
    que.Terminate();
    for (int i = 0; i < 4; i++)
    {
        workers[i]->join();
    }
}

#endif // _USE_GTEST
