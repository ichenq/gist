// Copyright of BeyondTech, Inc. 2014-2016. All rights reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form 
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially 
// exploit the content. Nor may you transmit it or store it in any other website or 
// other form of electronic retrieval system.

#pragma once

#include <assert.h>

template <typename T>
ChannelQueue<T>::ChannelQueue(uint32_t capacity)
    : bounds_(capacity)
{
    data_ = new T[bounds_];
}

template <typename T>
ChannelQueue<T>::~ChannelQueue()
{
    delete[] data_;
}


// Push new data onto the queue. Blocks if the queue is full.
// Once the push operation has completed, it signals other threads waiting
// in `ChannelQueue::Pop()` that they may continue consuming sockets.
template <typename T>
Status ChannelQueue<T>::Push(const T& value)
{
    if (terminated_)
    {
        return APR_EOF; // no more elements ever again
    }
    std::unique_lock<std::mutex> lock(one_big_mutex_);
    if (IsFull())
    {
        if (!terminated_)
        {
            full_waiters_++;
            not_full_.wait(lock);
            full_waiters_--;
        }
        // If we wake up and it's still empty, then we were interrupted
        if (IsFull())
        {
            if (terminated_)
            {
                return APR_EOF;
            }
            return APR_EINTER;
        }
    }
    data_[in_++] = value;
    if (in_ >= bounds_)
    {
        in_ -= bounds_;
    }
    nelts_++;
    if (empty_waiters_ > 0)
    {
        not_empty_.notify_one();
    }
    return APR_SUCCESS;
}


// Push new data onto the queue. If the queue is full, return APR_EAGAIN.
// If the push operation completes successfully, it signals other threads 
// waiting in `ChannelQueue::Pop()` that they may continue consuming sockets.
template <typename T>
Status ChannelQueue<T>::TryPush(const T& value)
{
    if (terminated_)
    {
        return APR_EOF; // no more elements ever again
    }
    std::unique_lock<std::mutex> lock(one_big_mutex_);
    if (IsFull())
    {
        return APR_EAGAIN;
    }
    data_[in_++] = value;
    if (in_ >= bounds_)
    {
        in_ -= bounds_;
    }
    nelts_++;
    if (empty_waiters_ > 0)
    {
        not_empty_.notify_one();
    }
    return APR_SUCCESS;
}

// Retrieves the next item from the queue. 
// If there are no items available, it will block until one becomes available.
// Once retrieved, the item is placed into the address specified by 'ptr'.
template <typename T>
Status ChannelQueue<T>::Pop(T* ptr)
{
    assert(ptr != nullptr);
    if (terminated_)
    {
        return APR_EOF; // no more elements ever again
    }
    std::unique_lock<std::mutex> lock(one_big_mutex_);
    if (IsEmpty())
    {
        if (!terminated_)
        {
            empty_waiters_++;
            not_empty_.wait(lock);
            empty_waiters_--;
        }
        // If we wake up and it's still empty, then we were interrupted
        if (IsEmpty())
        {
            if (terminated_)
            {
                return APR_EOF;
            }
            return APR_EINTER;
        }
    }
    *ptr = data_[out_++];
    nelts_--;
    if (out_ >= bounds_)
    {
        out_ -= bounds_;
    }
    if (full_waiters_ > 0)
    {
        not_full_.notify_one();
    }
    return APR_SUCCESS;
}


// Retrieves the next item from the queue. 
// If there are no items available, return APR_EAGAIN.  
// Once retrieved, the item is placed into the address specified by 'ptr'.
template <typename T>
Status ChannelQueue<T>::TryPop(T* ptr)
{
    assert(ptr != nullptr);
    if (terminated_)
    {
        return APR_EOF;
    }
    std::unique_lock<std::mutex> lock(one_big_mutex_);
    if (IsEmpty())
    {
        return APR_EAGAIN;
    }
    *ptr = data_[out_++];
    nelts_--;
    if (out_ >= bounds_)
    {
        out_ -= bounds_;
    }
    if (full_waiters_ > 0)
    {
        not_full_.notify_one();
    }
    return APR_SUCCESS;
}

template <typename T>
void ChannelQueue<T>::InterruptAll()
{
    std::unique_lock<std::mutex> lock(one_big_mutex_);
    not_empty_.notify_all();
    not_full_.notify_all();
}

template <typename T>
void ChannelQueue<T>::Terminate()
{
    {
        std::lock_guard<std::mutex> guard(one_big_mutex_);
        // we must hold `one_big_mutex` when setting this... otherwise,
        // we could end up setting it and waking everybody up just after a
        // would-be popper checks it but right before they block
        terminated_ = true;
    }
    InterruptAll();
}

template <typename T>
void ChannelQueue<T>::Reset()
{
    std::lock_guard<std::mutex> guard(one_big_mutex_);
    terminated_ = false;
    in_ = 0;
    out_ = 0;
    full_waiters_ = 0;
    empty_waiters_ = 0;
}