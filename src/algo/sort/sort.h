// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#pragma once

#include <algorithm>  // std::swap

// Bubble Sort is the simplest sorting algorithm that works by repeatedly swapping 
// the adjacent elements if they are in wrong order.
//  Worst-case performance	 §°(n2) comparisons, §°(n2) swaps
//  Best-case performance	 §°(n)  comparisons, §°(1) swaps
//  Average performance	     §°(n2) comparisons, §°(n2) swaps
template <typename T>
void bubbleSort(T a[], int n)
{
    for (int i = 0; i < n; i++) 
    {
        bool swapped = false;
        // Last i elements are already in place  
        for (int j = 0; j < n - i - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                std::swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)  // no data exchange
            break;
    }
}

// The selection sort algorithm sorts an array by repeatedly finding the minimum 
// element(considering ascending order) from unsorted part and putting it at the beginning
//      Worst-case performance	§°(n2) comparisons, §°(n) swaps
//      Best-case performance	§°(n2) comparisons, §°(n) swaps
//      Average performance	    §°(n2) comparisons, §°(n) swaps
template <typename T>
void selectionSort(T a[], int n)
{
    int min_idx = -1;
    // One by one move boundary of unsorted subarray
    for (int i = 0; i < n - 1; i++)
    {
        min_idx = i;
        // Find the minimum element in unsorted array 
        for (int j = i + 1; j < n; j++)
        {
            if (a[j] < a[min_idx])
                min_idx = j;
        }
        if (min_idx != i)
            std::swap(a[min_idx], a[i]);
    }
}

// Insertion sort virtually split the array into a sorted and an unsorted part.  
// Values from the unsorted part are picked and placed at the correct position in the sorted part.
// Worst-case performance	§°(n2) comparisons and swaps
// Best-case performance	O(n)  comparisons, O(1) swaps
// Average performance	    §°(n2) comparisons and swaps
// Advantages:
//	1) simple to implement
//	2) effcient for small data sets
//	3) adaptive && online
//	4) stable
template <typename T>
void insertionSort(T a[], int n)
{
    for (int i = 1; i < n; i++)
    {
        T x = a[i];
        int j = i - 1;
        // Move elements of arr[0..i-1], that are greater than key, 
        // to one position ahead of their current position
        while (j >= 0 && a[j] > x)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = x;
    }
}

template <typename T>
void shellSort(T a[], int n)
{
    // Marcin Ciura's gap sequence
    const int gaps[] = { 701, 301, 132, 57, 23, 10, 4, 1 };
    for (int gap : gaps)
    {
        // The first gap elements a[0..gap-1] are already in gapped order
        // keep adding one more element until the entire array is gap sorted
        for (int i = gap; i < n; i++)
        {
            T tmp = a[i];
            int j = i;
            // shift earlier gap-sorted elements up until the correct location for a[i] is found
            for (; j >= gap && a[j - gap] > tmp; j -= gap)
                a[j] = a[j - gap];
            a[j] = tmp;
        }
    }
}

// reorder the array so that all elements with values less than the pivot 
// come before the pivot, while all elements with values greater than the 
// pivot come after it (equal values can go either way). 
template <typename T>
int partition(T arr[], int lo, int hi)
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
void quickSortImpl(T arr[], int lo, int hi)
{
    if (lo < hi) {
        int pv = partition(arr, lo, hi);
        quickSortImpl(arr, lo, pv - 1);
        quickSortImpl(arr, pv + 1, hi);
    }
}

template <typename T>
void quickSort(T arr[], int n)
{
    quickSortImpl(arr, 0, n - 1);
}