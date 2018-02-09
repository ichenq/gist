// Copyright (C) 2018 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License. 
// See accompanying files LICENSE.

#pragma once

#include <stdint.h>
#include <unordered_map>
#include "List.h"

template <typename K>
class LRUCache
{
public:
    LRUCache(int capacity = INT_MAX)
        : capacity_(capacity)
    {
        dict_.rehash(16);
    }

    ~LRUCache()
    {

    }

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    int Size() const { return size_; }

    bool Exist(const K& key) const 
    {
        return dict_.count(key) > 0;
    }

    ListHead* Get(const K& key)
    {
        auto iter = dict_.find(key);
        if (iter != dict_.end())
        {
            ListHead* item = iter.second;
            moveToListFront(item);
            return item;
        }
        return nullptr;
    }

    void Put(const K& key, ListHead* item)
    {
        auto iter = dict_.find(key);
        if (iter != dict_.end())
        {
            ListHead* node = iter->second;
            node->Replace(item);
            moveToListFront(node);
        }
        else
        {
            head.AddTail(item);
            dict_[key] = item;
            size_++;
        }
    }

private:
    void shrink()
    {
        while (size_ > capacity_)
        {
            ListHead* node = head.prev; // back element
            node->Del();
            dict_.erase(node->);
        }
    }

    void moveToListFront(ListHead* item)
    {

    }


private:
    const int capacity_;
    int size_ = 0;
    ListHead head;
    std::unordered_map<K, ListHead*> dict_;
};