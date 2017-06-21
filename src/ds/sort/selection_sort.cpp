// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

// selection sort

#include <assert.h>
#include <algorithm>

// O(N^2) complexity
// step 1: 找到数组中最小元素并将其和数组第一个元素交换位置
// step 2: 在剩下的元素中找到最小元素并将其与数组第二个元素交换，直至整个数组排序
template <typename T>
void SelectionSort(T* p, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        // assume min is the first element
        int iMin = i;
        for (int j = i + 1; j < size; j++)
        {
            if (p[j] < p[iMin])
            {
                iMin = j;
            }
        }
        if (iMin != i)
        {
            std::swap(p[i], p[iMin]);
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
    SelectionSort(array, N);
    for (int i = 0; i < N - 1; i++) {
        assert(array[i] < array[i + 1]);
    }
}
