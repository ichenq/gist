// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#pragma once

#include <vector>
#include <algorithm>    // std::min

// Merge sort with both top-down and bottom-up
// Complexity: O(N*logN)
// http://algs4.cs.princeton.edu/22mergesort/

template <typename T>
void top_down_merge(T arr[], T tmp[], int first, int mid, int last)
{
    int i = first, j = mid + 1;
    int k = 0;
    while (i <= mid && j <= last) {
        if (arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];
        } else {
            tmp[k++] = arr[j++];
        }
    }
    while (i <= mid) { // rest of first half
        tmp[k++] = arr[i++];
    }
    while (j <= last) { // rest of right half
        tmp[k++] = arr[j++];
    }
    // copy back to `arr`
    for (i = 0; i < k; i++) {
        arr[first + i] = tmp[i];
    }
}

template <typename T>
void top_down_merge_sort(T arr[], T tmp[],  int left, int right)
{
    if (left < right) {
        int mid = (left + right) / 2;
        top_down_merge_sort(arr, tmp, left, mid);
        top_down_merge_sort(arr, tmp, mid + 1, right);
        top_down_merge(arr, tmp, left, mid, right);
    }
}

//////////////////////////////////////////////////////////////////////////////

template <typename T>
void bottom_up_merge(T A[], T B[], int left, int right, int end)
{
    int i = left;
    int j = right;
    // While there are elements in the left or right runs...
    for (int k = left; k < end; k++) {
        // If left run head exists and is <= existing right run head.
        if (i < right && (j >= end || A[i] <= A[j])) {
            B[k] = A[i];
            i++;
        } else {
            B[k] = A[j];
            j++;
        }
    }
}

template <typename T>
void bottom_up_merge_sort(T A[], T B[], int n)
{
    // Each 1-element run in A is already "sorted".
    // Make successively longer sorted runs of length 2, 4, 8, 16... 
    // until whole array is sorted.
    for (int width = 1; width < n; width *= 2) {
        // Array A is full of runs of length width.
        for (int i = 0; i < n; i = i + 2 * width) {
            // Merge two runs: A[i:i+width-1] and A[i+width:i+2*width-1] to B[]
            // or copy A[i:n-1] to B[] ( if(i+width >= n) )
            bottom_up_merge(A, B, i, std::min(i+width, n), std::min(i+2*width, n));
        }
        // Now work array B is full of runs of length 2*width.
        // Copy array B to array A for next iteration.
        // A more efficient implementation would swap the roles of A and B.
        for (int i = 0; i < n; i++) {
            A[i] = B[i];
        }
    }
}

// Worst-case:  O(N logN)
// Best-case:   O(N logN) - O(N)
// Average:     O(N logN)
template <typename T>
void merge_sort(T arr[], int size)
{
    if (size <= 1) {
        return;
    }
    std::vector<T> tmp;
    tmp.resize(size);
    top_down_merge_sort(arr, (T*)tmp.data(), 0, size - 1);
    //bottom_up_merge_sort(arr, tmp, size);
}
