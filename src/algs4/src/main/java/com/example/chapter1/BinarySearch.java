package com.example.chapter1;

import java.util.*;
import edu.princeton.cs.algs4.*;

public class BinarySearch {

    public static int search(int key, int[] a)
    {
        int lo = 0;
        int hi = a.length - 1;
        while (lo <= hi)
        {
            int mid = lo + (hi - lo) / 2;
            if (a[mid] == key) {
                return mid;
            }
            else if (a[mid] > key) {
                hi = mid - 1;
            }
            else if (a[mid] < key) {
                lo = mid + 1;
            }
        }
        return -1;
    }

    // Exercise 1.3.11
    // 返回小于该key的元素数量, lower_bound
    public static int rank(int key, int[] a)
    {
        int lo = 0;
        int hi = a.length;
        while (lo < hi)
        {
            int mid = lo + (hi - lo) / 2;
            if (a[mid] >= key) {
                hi = mid;
            }
            else if (a[mid] < key) {
                lo = mid + 1;
            }
        }
        return lo;
    }

    // 小于等于key的数量
    public static int upper_bound(int key, int[] a)
    {
        int lo = 0;
        int hi = a.length;
        while (lo < hi)
        {
            int mid = lo + (hi - lo) / 2;
            if (a[mid] <= key) {
                lo = mid + 1;
            }
            else if (a[mid] > key) {
                hi = mid;
            }
        }
        return hi - 1;
    }

    //
    public static int count(int key, int[] a)
    {
        int start = rank(key, a);
        if (start >= a.length) {
            return 0;
        }
        int end = upper_bound(key, a);
        if (end < 0) {
            return 0;
        }
        return end - start + 1;
    }

    public static void main(String args[])
    {
//        int[] whitelist = StdIn.readAllInts();
//        Arrays.sort(whitelist);
//        while (!StdIn.isEmpty())
//        {
//            int key = StdIn.readInt();
//            if (search(key, whitelist) < 0) {
//                StdOut.println(key);
//            }
//        }

        int[] arr = {1, 3, 3, 4, 7, 9};
        int[] to = {0, 1, 2, 8, 9, 10};
        for (int i = 0; i <= 10; i++) {
            int c = count(i, arr);
            System.out.println(String.format("count of %d: %d", i, c));
        }
    }
}
