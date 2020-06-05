// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

//
// 有一组N个数，找出其中的第k个最大者
//

#include <assert.h>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include "binary_heap.h"

// O(N^2) complexity
// as simple as this code is
int MaxKthElementV1(const int input[], int N, int k)
{
    assert(k >= 0 && k < N);
    std::vector<int> vec(input, input + N);
    std::stable_sort(vec.begin(), vec.end(), std::greater<int>());
    return vec[k-1];
}

// O(N*K) best case, O(N^2) worst case
// use a sorted [0, k) range
int MaxKthElementV2(const int input[], int N, int k)
{
    assert(k >= 0 && k < N);
    std::vector<int> vec(input, input + k);
    std::stable_sort(vec.begin(), vec.end(), std::greater<int>()); // sort range [0,k) 
    for (int i = k; i < N; i++) {
        int v = input[i];
        if (v > vec[k - 1]) {
            vec[k - 1] = v;
            // insertion sort
            for (int m = 1; m < k; m++) {
                for (int j = m; j > 0 && vec[j - 1] < vec[j]; j--) {
                    std::swap(vec[j], vec[j - 1]);
                }
            }
        }
    }
    return vec[k - 1];
}

// O(N*logN)
// use a maximum binary heap
int MaxKthElementV3(const int input[], int N, int k)
{
    assert(k >= 0 && k < N);
    std::vector<int> vec(input, input + N);
    heapify(&vec[0], N, std::greater<int>()); // max-heap
    for (int i = 0; i < k - 1; i++) { // k times heap delete root
        int n = vec.size() - 1;
        std::swap(vec[0], vec[n]);
        heapDown(&vec[0], 0, n, std::greater<int>());
        vec.pop_back();
    }
    return vec[0];
}

// O(N*logK)
// use a minimum binary heap
int MaxKthElementV4(const int input[], int N, int k)
{
    assert(k >= 0 && k < N);
    std::vector<int> vec(input, input + k);
    heapify(&vec[0], k, std::less<int>()); // min-heap
    for (int i = k; i < N; i++) {
        if (input[i] > vec[0]) {
            // heap push
            vec.push_back(input[i]);
            int n = vec.size() - 1;
            heapUp(&vec[0], n, std::less<int>());

            // heap pop
            std::swap(vec[0], vec[n]);
            heapDown(&vec[0], 0, n, std::less<int>());
            vec.pop_back();
        }
    }
    return vec[0];
}

void simple_test()
{
    std::mt19937 prg(123456789);
    std::vector<int> array;
    for (int i = 0; i < 10000; i++) {
        array.push_back(prg() % 1000000);
    }

    const int K = 12;
    int v1 = MaxKthElementV1(&array[0], array.size(), K);
    int v2 = MaxKthElementV2(&array[0], array.size(), K);
    int v3 = MaxKthElementV3(&array[0], array.size(), K);
    int v4 = MaxKthElementV4(&array[0], array.size(), K);
    assert(v1 == v2 && v2 == v3 && v3 == v4);
}
