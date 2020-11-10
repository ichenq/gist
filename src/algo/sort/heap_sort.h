// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#pragma once

#include <assert.h>
#include <utility>
#include <functional>
#include "binary_heap.h"

// Complexity N*log(N)
template <typename T>
void heap_sort(T arr[], int size)
{
    int n = size;
	// build heap with greatest element at top.
    heapify(arr, n, std::greater<T>());
	
	// pop elements, largest first, into end of data.
    while (n-- > 0) {
        std::swap(arr[0], arr[n]);
        heap_down(&arr[0], 0, n, std::greater<T>());
    }
}
