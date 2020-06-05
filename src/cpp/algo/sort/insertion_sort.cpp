// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

// insertion sort

#include <assert.h>
#include <algorithm>

// Complexity: 
//	O(N^2) worst case
//	O(N) best case
// Advantages:
//	1) simple to implement
//	2) effcient for small data sets
//	3) adaptive && online
//	4) stable
template <typename T>
void InsertionSort(T* p, int size)
{
    for (int i = 1; i < size; i++)
    {
        for (int j = i; j > 0 && p[j - 1] > p[j]; j--)
        {
            std::swap(p[j], p[j - 1]);
        }
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
    InsertionSort(array, N);
    for (int i = 0; i < N - 1; i++) {
        assert(array[i] < array[i + 1]);
    }
}
