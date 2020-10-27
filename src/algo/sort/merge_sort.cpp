// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <assert.h>
#include <vector>
#include <algorithm>

// Merge sort with both top-down and bottom-up
// Complexity: O(N*logN)
// http://algs4.cs.princeton.edu/22mergesort/

template <typename T>
void topDownMerge(std::vector<T>& arr, std::vector<T>& tmp, int first, int mid, int last)
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
void TopDownMergeSort(std::vector<T>& arr, std::vector<T>& tmp,  int left, int right)
{
    if (left < right) {
        int mid = (left + right) / 2;
        TopDownMergeSort(arr, tmp, left, mid);
        TopDownMergeSort(arr, tmp, mid + 1, right);
        topDownMerge(arr, tmp, left, mid, right);
    }
}

//////////////////////////////////////////////////////////////////////////////

template <typename T>
void bottomUpMerge(std::vector<T>& A, std::vector<T>& B, int left, int right, int end)
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
void bottomUpMergeSort(std::vector<T>& A, std::vector<T>& B, int n)
{
    // Each 1-element run in A is already "sorted".
    // Make successively longer sorted runs of length 2, 4, 8, 16... 
    // until whole array is sorted.
    for (int width = 1; width < n; width *= 2) {
        // Array A is full of runs of length width.
        for (int i = 0; i < n; i = i + 2 * width) {
            // Merge two runs: A[i:i+width-1] and A[i+width:i+2*width-1] to B[]
            // or copy A[i:n-1] to B[] ( if(i+width >= n) )
            bottomUpMerge(A, B, i, std::min(i+width, n), std::min(i+2*width, n));
        }
        // Now work array B is full of runs of length 2*width.
        // Copy array B to array A for next iteration.
        // A more efficient implementation would swap the roles of A and B.
        for (int i = 0; i < n; i++) {
            A[i] = B[i];
        }
    }
}

template <typename T>
void MergeSort(std::vector<T>& arr)
{
    size_t size = arr.size();
    if (size > 1) {
        std::vector<T> tmp;
        tmp.resize(size);
        TopDownMergeSort(arr, tmp, 0, size - 1);
        //bottomUpMergeSort(arr, tmp, size);
    }
}

void simple_test()
{
    std::vector<int> vec {
        83, 21, 93, 24, 40, 23, 46,
        75, 95, 93, 41, 51, 3, 79,
        67, 54, 
    };
    const int N = vec.size();
    MergeSort(vec);
    for (int i = 0; i < N - 1; i++) {
        assert(vec[i] < vec[i + 1]);
    }
}
