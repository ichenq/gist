// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#pragma once


// find target in [0, size)
template <typename T>
int binary_search(const T arr[], int size, const T& target)
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

// return [0, size)
template <typename T>
int binary_search_lower_bound(const T arr[], int size, const T& target)
{
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

// returns [0, size]
template <typename T>
int binary_search_upper_bound(const T arr[], int size, const T& target)
{
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
    // `right` indicates how many number less than or equal to `target`
    return right;
}
