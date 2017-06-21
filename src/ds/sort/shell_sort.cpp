// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <assert.h>

// Complexity: 
//   N*log2(N^2) worst case
//   N*log(N)    best case 
template <typename T>
void ShellSort(T* p, int size)
{
    // Marcin Ciura's gap sequence
    const int gaps[] = { 701, 301, 132, 57, 23, 10, 4, 1 };
    for (int gap : gaps)
    {
        for (int i = gap; i < size; i++)
        {
            auto tmp = p[i];
            int j = i;
            for (; j >= gap && p[j - gap] > tmp; j -= gap)
            {
                p[j] = p[j - gap];
            }
            p[j] = tmp;
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
    ShellSort(array, N);
    for (int i = 0; i < N - 1; i++) {
        assert(array[i] < array[i + 1]);
    }
}
