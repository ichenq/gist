package com.example.chapter1;

import java.util.Arrays;
import java.util.Collections;

// Exercise 1.4.20 查询双调数组
public class BitonicSearch
{
    // 找到双调数组的峰点
    public static int findBitonicPoint(int[] arr)
    {
        int lo = 0;
        int hi = arr.length;
        while (lo < hi)
        {
            int mid = lo + (hi - lo) / 2;
            if (arr[mid] > arr[mid-1] && arr[mid] > arr[mid+1]) {
                return mid;
            } else if (arr[mid] > arr[mid-1]) {
                lo = mid + 1;
            } else if (arr[mid] > arr[mid+1]) {
                hi = mid;
            }
        }
        return -1;
    }

    // 升序二分查找, [lo, hi)
    public static int ascendingBinarySearch(int[] arr, int lo, int hi, int target)
    {
        while (lo < hi)
        {
            int mid = lo + (hi - lo) / 2;
            if (arr[mid] == target) {
                return mid;
            }
            else if (arr[mid] < target) {
                lo  = mid + 1;
            }
            else if (arr[mid] > target) {
                hi = mid;
            }
        }
        return -1;
    }

    // 降序二分查找, [lo, hi)
    public static int descendingBinarySearch(int[] arr, int lo, int hi, int target)
    {
        while (lo < hi)
        {
            int mid = lo + (hi - lo) / 2;
            if (arr[mid] == target) {
                return mid;
            }
            else if (arr[mid] > target) {
                lo = mid + 1;
            }
            else if (arr[mid] < target) {
                hi = mid;
            }
        }
        return -1;
    }

    public static int search(int[] arr, int target)
    {
        // complexity 3*log(N)
        if (false) {
            int bp = findBitonicPoint(arr);
            int n = ascendingBinarySearch(arr, 0, bp, target);
            if (n < 0) {
                n = descendingBinarySearch(arr, bp, arr.length, target);
            }
            return n;
        } else {
            // complexity 2*log(N)
            return bitonicSearch(arr, 0, arr.length, target);
        }
    }

    public static int bitonicSearch(int[] arr, int lo, int hi, int target)
    {
        if (lo > hi) {
            return -1;
        }
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < target) {
            //如果在上升区间，继续查找右侧
            if (arr[mid] <= arr[mid+1]) {
                return bitonicSearch(arr, mid+1, hi, target);
            }
            else { //如果在下降区间，继续查找左侧
                return bitonicSearch(arr, lo, mid-1, target);
            }
        }
        else
        {
            int idx = ascendingBinarySearch(arr, lo, mid, target);
            if (idx < 0) {
                idx = descendingBinarySearch(arr, mid, hi, target);
            }
            return idx;
        }
    }

    public static void main(String[] args)
    {
        int[] bitonicArr = {1,25,19,37,12,74,35,21,54,61,80,93, 80, 74, 61, 54, 37, 35, 25, 21, 19, 12, 1,};

        System.out.println("array is:");
        for (int n : bitonicArr) {
            System.out.print(n + ", ");
        }
        System.out.println("");

        int[] targets = {1, 19, 93, 11, 21, 10, 0};
        for (int n : targets) {
            System.out.print(String.format("find %d in array: ", n));
            int idx = BitonicSearch.search(bitonicArr, n);
            if (idx >= 0) {
                System.out.println(String.format("found %d at index %d", bitonicArr[idx], idx));
            } else {
                System.out.println("not found");
            }
        }
    }
}