// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#pragma once

#include <assert.h>

// Find value in range [low, high]
template <typename T>
int BinarySearch(const T* a, int low, int high, const T& value)
{
    // special case
    if (low == high) { 
        return a[low] == value ? 0 : -1;
    }
    while (low <= high) {
        int mid = (low + high) / 2;
        if (a[mid] < value) {
            low = mid + 1;
        }
        else if (value < a[mid]) {
            high = mid;
        }
        else {
            return mid;     // found
        }
    }
    return -1; // not found
}

void unit_test()
{
    int array[] = { 1, 2, 3, 4, 5, 6, 7 };
    int idx = 0;
    idx = BinarySearch(array, 0, 7, 4);
    assert(idx == 3);
    idx = BinarySearch(array, 0, 0, 0);
    assert(idx == -1);
    idx = BinarySearch(array, 0, 1, 1);
    assert(idx == 0);
}