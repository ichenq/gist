// Copyright (C) 2018 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License. 
// See accompanying files LICENSE.

#pragma once

// Simple doubly linked list implementation.
struct ListHead 
{
    ListHead* next;
    ListHead* prev;

    ListHead()
    {
        reset();
    }

    void reset()
    {
        next = this;
        prev = this;
    }

    // Insert a new entry between two known consecutive entries.
    void AddTo(ListHead* prev, ListHead* next)
    {
        next->prev = this;
        this->next = next;
        this->prev = prev;
        prev->next = this;
    }

    // Insert a new entry before this head.
    void AddTail(ListHead* item)
    {
        item->AddTo(this->prev, this);
    }

    // Deletes entry from list.
    void Del()
    {
        next->prev = prev;
        prev->next = next;
        reset();
    }

    void Replace(ListHead* item)
    {
        next->prev = item;
        item->next = next;
        item->prev = prev;
        prev->next = item;
        reset();
    }
};