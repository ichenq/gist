package com.example.chapter2;

import edu.princeton.cs.algs4.StdRandom;

public class SortUtil
{
    public static boolean less(Comparable a, Comparable b)
    {
        return a.compareTo(b) < 0;
    }

    public static void exch(Comparable[] arr, int i, int j)
    {
        Comparable tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }

    public static boolean isSorted(Comparable[] arr)
    {
        for (int i = 1; i < arr.length; i++)
        {
            if (less(arr[i], arr[i-1]))
                return false;
        }
        return true;
    }

    public static boolean isSorted(int[] arr)
    {
        for (int i = 1; i < arr.length; i++)
        {
            if (less(arr[i], arr[i-1]))
                return false;
        }
        return true;
    }

    public static Integer[] randomIntArray(int N)
    {
        Integer[] arr = new Integer[N];
        for (int i = 0; i < N; i++) {
            arr[i] = StdRandom.uniform(100);
        }
        return arr;
    }
}