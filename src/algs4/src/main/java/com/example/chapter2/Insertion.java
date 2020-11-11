package com.example.chapter2;

import edu.princeton.cs.algs4.*;


// 插入排序
public class Insertion
{
    // Exercise 2.1.26 原始int类型版本
    public static void sortInt(int[] arr)
    {
        for (int i = 1; i < arr.length; i++)
        {
            for (int j = i; j > 0 && arr[j] > arr[j-1]; j--)
            {
                int tmp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1] = tmp;
            }
        }
    }

    public static void sort(Comparable[] arr)
    {
        for (int i = 1; i < arr.length; i++)
        {
            int j = i - 1;
            Comparable curr = arr[i];
            while (j >= 0 && arr[j].compareTo(curr) > 0) {
                arr[j+1] = arr[j];
                j--;
            }
            arr[j+1] = curr;
        }
    }


    // Exercise 2.1.24 哨兵，去掉内循环中j>0的判断
    public static void sortSentinel(Comparable[] arr)
    {
        //
        int min = 0;
        for (int i = 1; i < arr.length; i++) {
            if (arr[i].compareTo(arr[min]) < 0) {
                min = i;
            }
        }
        if (min != 0) {
            SortUtil.exch(arr, min, 0);
        }
        for (int i = 1; i < arr.length - 1; i++)
        {
            for (int j = i; SortUtil.less(arr[j], arr[j-1]); j--)
            {
                SortUtil.exch(arr, j, j-1);
            }
        }
    }

    // 小于等于key的index [0, N]
    public static int upper_bound(Comparable[] arr, int hi, Comparable key)
    {
        int lo = 0;
        while (lo < hi)
        {
            int mid = lo + (hi - lo) / 2;
            int cmp = arr[mid].compareTo(key);
            if (cmp <= 0) {
                lo = mid + 1;
            }
            else if (cmp > 0) {
                hi = mid;
            }
        }
        return hi;
    }

    // 代入二分查找的插入排序
    public static void sortBinSearch(Comparable[] arr)
    {
        for (int i = 1; i < arr.length; i++)
        {
            int idx = upper_bound(arr, i, arr[i]);
            if (idx != i)
            {
                Comparable tmp = arr[i];
                int j = i - 1;
                for (; j >= idx; j--) {
                    arr[j+1] = arr[j];
                }
                arr[idx] = tmp;
            }
        }
    }

    // 递归版本
    public static void sort_recursive(Comparable[] arr, int n)
    {
        // base case
        if (n <= 1)
            return ;

        // sort first n-1 elements
        sort_recursive(arr, n-1);

        // insert last element to sorted array
        Comparable last = arr[n-1];
        int j = n - 2;
        while (j >= 0 && SortUtil.less(last, arr[j])) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = last;
    }

    public static double benchmarkSort(int N, int T, int method)
    {
        double total = 0.0;
        Double[] arr = new Double[N];
        for (int t = 0; t < T; t++)
        {
            for (int i = 0; i < N; i++) {
                arr[i] = StdRandom.uniform() * 100;
            }
            Stopwatch timer = new Stopwatch();
            if (method == 1) {
                Insertion.sort(arr);
            } else if (method == 2) {
                Insertion.sortSentinel(arr);
            } else if (method == 3) {
                Insertion.sortBinSearch(arr);
            }
            total += timer.elapsedTime();
        }
        return total;
    }

    public static void showBenchmarks(int N, int T)
    {
        double t1 = benchmarkSort(N, T, 1);
        double t2 = benchmarkSort(N, T, 2);
        double t3 = benchmarkSort(N, T, 3);
        StdOut.printf("direct insertion sort: %.2f", t1);
        StdOut.printf("sentinal insertion sort: %.2f", t2);
        StdOut.printf("binsearch insertion sort: %.2f", t3);
    }

    public static void main(String[] args)
    {
        int N = Integer.parseInt(args[0]);
        int T = Integer.parseInt(args[1]);
        showBenchmarks(N, T);
    }
}