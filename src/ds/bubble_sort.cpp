// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

// bubble sort
// details see https://en.wikipedia.org/wiki/Bubble_sort

#include <assert.h>
#include <algorithm>

// bubble sort has worst-case and average complexity both О(n2)
template <typename T>
void BubbleSort(T* p, int size) 
{
    for (int n = size; n >= 0; n--) 
	{
        for (int i = 1; i < n; i++) 
		{
            if (p[i - 1] > p[i])
            {
                std::swap(p[i - 1], p[i]);
            }
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
    BubbleSort(array, N);
    for (int i = 0; i < N-1; i++) 
	{
        assert(array[i] < array[i+1]);
    }
}
