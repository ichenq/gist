package com.example.chapter2;

import edu.princeton.cs.algs4.StdRandom;

import java.util.Arrays;
import java.util.Collections;
import java.util.Random;

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

    public static void randomShuffleArray(Comparable[] arr)
    {
        Random rnd = new Random();
        for (int i = 0; i < arr.length; i++) {
            int r = rnd.nextInt(i+1);
            Comparable tmp = arr[r];
            arr[r] = arr[i];
            arr[i] = tmp;
        }
    }

    // Exercise 2.1.36
    // 生成不均匀额数据 一半为0，一半为1
    public static Integer[] randomUnUniformArray1(int N)
    {
        Integer[] arr = new Integer[N];
        for (int i = 0; i < N/2; i++) {
            arr[i] = 0;
            arr[N-i-1] = 1;
        }
        randomShuffleArray(arr);
        return arr;
    }

    // 生成不均匀额数据 一半为0，1/4为1， 1/4为2
    public static Integer[] randomUnUniformArray2(int N)
    {
        Integer[] arr = new Integer[N];
        int i = 0;
        for (; i < N/2; i++) {
            arr[i] = 0;
        }
        int n = i + N / 4;
        for (; i < n; i++) {
            arr[n] = 1;
        }
        for (; i < N; i++) {
            arr[n] = 2;
        }
        randomShuffleArray(arr);
        return arr;
    }

    // 生成不均匀额数据 一半为0，一半是随机int值
    public static Integer[] randomUnUniformArray3(int N)
    {
        Random rnd = new Random();
        Integer[] arr = new Integer[N];
        int i = 0;
        for (; i < N/2; i++) {
            arr[i] = 0;
        }
        for (; i < N; i++) {
            arr[i] = rnd.nextInt();
        }
        randomShuffleArray(arr);
        return arr;
    }

    // Exercise 2.1.36
    // 95%有序，其余部分为随机值
    public static Integer[] randomPartialOrderedArray(int N)
    {
        Random rnd = new Random();
        Integer[] arr = new Integer[N];
        int ordered_count = N * 95 / 100;
        for (int i = 0; i < ordered_count; i++)
        {
            arr[i] = rnd.nextInt();
        }
        Arrays.sort(arr);
        for (int i = 0; i < N - ordered_count; i++) {
            arr[i] = rnd.nextInt();
        }
        return arr;
    }
}