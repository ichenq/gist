// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <assert.h>
#include <vector>
#include <utility>
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
        int n = array_.size();
        for (int i = n / 2 - 1; i >= 0; i--) {
            down(i, n);
        }
    }

    ~BinaryHeap()
    {
    }

    size_t Size() const 
    {
        return array_.size();
    }

    // three steps to insert an item:
    //  1) Add the element to the bottom level of the heap.
    //  2) Compare the added element with its parent; if they are in the correct order, stop.
    //  3) If not, swap the element with its parent and return to the previous step.
    void Push(const T& v)
    {
        array_.push_back(v);
        up(array_.size() - 1);
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
        down(0, n);
        T v = array_[n];
        array_.pop_back();
        return v;
    }

private:
    void up(int j)
    {
        for (;;) {
            int i = (j - 1) / 2; // parent
            if (i == j || !(array_[j] < array_[i])) {
                break;
            }
            std::swap(array_[i], array_[j]);
            j = i;
        }
    }

    bool down(int i0, int n)
    {
        int i = i0;
        for (;;) {
            int j1 = 2 * i + 1;
            if (j1 >= n || j1 < 0) { // j1 < 0 after int overflow
                break;
            }
            int j = j1; // left child
            int j2 = j1 + 1; 
            if (j2 < n && !(array_[j1] < array_[j2])) {
                j = j2; // = 2 * i + 2 // right child
            }
            if (!(array_[j] < array_[i])) {
                break;
            }
            std::swap(array_[i], array_[j]);
            i = j;
        }
        return i > i0;
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
