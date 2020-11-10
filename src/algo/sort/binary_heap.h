// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#pragma once

#include <algorithm>    // std::swap

// min heap operation
template <typename P, typename Pred>
void heap_up(P arr[], int j, Pred less)
{
    for (;;) {
        int i = (j - 1) / 2; // parent
        if (i == j || !less(arr[j], arr[i])) {
            break;
        }
        std::swap(arr[i], arr[j]);
        j = i;
    }
}

template <typename P, typename Pred>
bool heap_down(P arr[], int i0, int n, Pred less)
{
    int i = i0;
    for (;;) {
        int j1 = 2 * i + 1;
        if (j1 >= n || j1 < 0) { // j1 < 0 after int overflow
            break;
        }
        int j = j1; // left child
        int j2 = j1 + 1;
        if (j2 < n && !less(arr[j1], arr[j2])) {
            j = j2; // = 2 * i + 2 // right child
        }
        if (!less(arr[j], arr[i])) {
            break;
        }
        std::swap(arr[i], arr[j]);
        i = j;
    }
    return i > i0;
}

template <typename P, typename Pred>
void heapify(P arr[], int n, Pred less)
{
    for (int i = n / 2 - 1; i >= 0; i--) {
        heap_down(arr, i, n, less);
    }
}
