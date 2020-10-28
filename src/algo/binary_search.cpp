// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <assert.h>
#include <utility>
#include <initializer_list>

// returns [-1, size-1]
int binary_search(int arr[], int size, int target)
{
    int left = 0;
    int right = size - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        else if (arr[mid] < target) {
            left = mid + 1;   // continue search [mid+1, right]
        }
        else if (arr[mid] > target) {
            right = mid - 1;   // continue search [left, mid-1]
        }
    }
    return -1;
}

// returns [0, size]
int search_lower_bound(int arr[], int size, int target)
{
    if (size == 0) {
        return -1;
    }
    int left = 0;
    int right = size;
    while (left < right)
    {
        int mid = left + (right - left) / 2;
        if (arr[mid] >= target) {
            right = mid;    // continue search [left, mid)
        }
        else if (arr[mid] < target) {
            left = mid + 1;   // continue search [mid+1, right)
        } 
    }
    return left; // indicates how many number less than `target`
}

// returns [0, size-1]
int search_upper_bound(int arr[], int size, int target)
{
    if (size == 0) {
        return -1;
    }
    int left = 0;
    int right = size;
    while (left < right)
    {
        int mid = left + (right - left) / 2;
        if (arr[mid] <= target) {
            left = mid + 1;     // continue search [mid+1, right)
        }
        else if (arr[mid] > target) {
            right = mid;  // continue search [left, mid)
        }
    }
    // `right` indicates how many number less than or equal `target`
    return right - 1; 
}

void test_search()
{
    int arr1[] = { 1, 3, 5, 7, 11, 23, 31, 44 };

    std::pair<int, int> pairs1[] = {
        {0, -1},
        {1, 0},
        {5, 2},
        {31, 6},
        {44, 7},
        {50, -1},
    };
    for (const auto& item : pairs1)
    {
        int idx = binary_search(arr1, sizeof(arr1) / sizeof(arr1[0]), item.first);
        assert(idx == item.second);
    }

    int arr2[] = { 1, 3, 7, 7, 11, 31, 31, 34 };
    std::pair<int, int> pairs2[] = {
        {0, 0},
        {1, 0},
        {7, 2},
        {11, 4},
        {31, 5},
        {34, 7},
        {50, 8},
    };
    for (const auto& item : pairs2)
    {
        int idx = search_lower_bound(arr2, sizeof(arr2) / sizeof(arr2[0]), item.first);
        assert(idx == item.second);
    }
    std::pair<int, int> pairs3[] = {
        {0, -1},
        {1, 0},
        {7, 3},
        {11, 4},
        {31, 6},
        {34, 7},
        {40, 7},
    };
    for (const auto& item : pairs3)
    {
        int idx = search_upper_bound(arr2, sizeof(arr2) / sizeof(arr2[0]), item.first);
        assert(idx == item.second);
    }
}
