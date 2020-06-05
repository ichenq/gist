// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <assert.h>
#include <vector>
#include <algorithm>

// complexity: best O(N*logN), worst case O(N^2)
// http://algs4.cs.princeton.edu/23quicksort
// http://www.tomgsmith.com/quicksort/content/codeWalkthrough/


// reorder the array so that all elements with values less than the pivot 
// come before the pivot, while all elements with values greater than the 
// pivot come after it (equal values can go either way). 
template <typename T>
int partition(std::vector<T>& arr, int lo, int hi)
{
    const T& pivot = arr[lo];
    int pv = lo;
    for (int j = lo + 1; j <= hi; j++) {
        if (arr[j] <= pivot) {
            pv++;
            if (pv != j) {
                std::swap(arr[pv], arr[j]);
            }
        }
    }
    std::swap(arr[lo], arr[pv]); // put the pivot value in its final position
    return pv;
}

template <typename T>
void quickSort(std::vector<T>& arr, int lo, int hi)
{
    if (lo < hi) {
        int pv = partition(arr, lo, hi);
        quickSort(arr, lo, pv - 1);
        quickSort(arr, pv + 1, hi);
    }
}

template <typename T>
void QuickSort(std::vector<T>& arr, int size)
{
    quickSort(arr, 0, size - 1);
}

void simple_test()
{
    std::vector<int> arr {
        83, 21, 93, 24, 40, 23, 46,
        75, 95, 93, 41, 51, 3, 79,
        67, 54,
    };
    const int N = arr.size();
    QuickSort(arr, N);
    for (int i = 0; i < N - 1; i++) {
        assert(arr[i] < arr[i + 1]);
    }
}
