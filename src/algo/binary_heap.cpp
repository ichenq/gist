// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include "binary_heap.h"
#include <assert.h>
#include <vector>
#include <functional>
#include <initializer_list>


// binary min-heap
template <typename T>
class BinaryHeap
{
public:
    BinaryHeap()
    {
    }

    // heapify
    explicit BinaryHeap(std::initializer_list<T> l)
        : array_(l)
    {
        heapify(&array_[0], array_.size(), std::less<int>());
    }

    ~BinaryHeap()
    {
    }

    size_t Size() const 
    {
        return array_.size();
    }


    void Push(const T& v)
    {
        array_.push_back(v);
        heapUp(&array_[0], array_.size() - 1, std::less<int>());
    }

    // three steps to delete root item:
    //  1) Replace the root of the heap with the last element on the last level.
    //  2) Compare the new root with its children; if they are in the correct order, stop.
    //  3) If not, swap the element with one of its children and return to the previous step. 
    T Pop()
    {
        assert(!array_.empty());
        int n = array_.size() - 1;
        std::swap(array_[0], array_[n]);
        heapDown(&array_[0], 0, n, std::less<int>());
        T v = array_[n];
        array_.pop_back();
        return v;
    }

private:
    std::vector<T>  array_;
};


void simple_test()
{
    BinaryHeap<int> minHeap {
        83, 21, 93, 24, 40, 23, 46,
        75, 95, 93, 41, 51, 3, 79,
    };
    int array[] = {
        83, 21, 93, 24, 40, 23, 46,
        75, 95, 93, 41, 51, 3, 79,
        67, 54, 79, 82, 46, 80,
    };
    BinaryHeap<int> minHeap1;
    for (int a : array)
    {
        minHeap1.Push(a);
    }
    for (int i = 0; i < 10; i++)
    {
        minHeap1.Pop();
    }
}
