// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.


#include <assert.h>
#include <array>
#include <iostream>
#include "sort.h"


using namespace std;


template <typename T>
inline void printArray(T arr[], int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}



void sort_test()
{
    const size_t N = 20;
    std::array<int, N> arr {
        83, 21, 93, 24, 40, 23, 46,
        75, 95, 93, 41, 51, 3, 79,
        67, 54, 79, 82, 46, 80,
    };
    {
        auto a2 = arr;
        bubbleSort(a2.data(), a2.size());
        for (int i = 0; i < N - 1; i++)
        {
            assert(a2[i] < a2[i + 1]);
        }
    }
    
    {
        auto a2 = arr;
        selectionSort(a2.data(), a2.size());
        for (int i = 0; i < N - 1; i++)
        {
            assert(a2[i] < a2[i + 1]);
        }
    }

    {
        auto a2 = arr;
        insertionSort(a2.data(), a2.size());
        for (int i = 0; i < N - 1; i++)
        {
            assert(a2[i] < a2[i + 1]);
        }
    }

    //
    {
        auto a2 = arr;
        shellSort(a2.data(), a2.size());
        for (int i = 0; i < N - 1; i++)
        {
            assert(a2[i] < a2[i + 1]);
        }
    }

    {
        auto a2 = arr;
        quickSort(a2.data(), a2.size());
        for (int i = 0; i < N - 1; i++)
        {
            assert(a2[i] < a2[i + 1]);
        }
    }
}