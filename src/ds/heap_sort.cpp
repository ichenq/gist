// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <assert.h>
#include <utility>
#include <functional>
#include "binary_heap.h"

// Complexity N*log(N)
template <typename T>
void HeapSort(T* p, int size)
{
    int n = size;
    heapify(p, size, std::greater<T>());
    while (n-- > 0) {
        std::swap(p[0], p[n]);
        heapDown(&p[0], 0, n, std::greater<T>());
    }
}

void simple_test()
{
    int array[] = {
        83, 21, 93, 24, 40, 23, 46,
        75, 95, 93, 41, 51, 3, 79,
        67, 54, 79, 82, 46, 80,
    };
    const int N = sizeof(array) / sizeof(array[0]);
    HeapSort(array, N);
    for (int i = 0; i < N - 1; i++) {
        assert(array[i] < array[i + 1]);
    }
}
