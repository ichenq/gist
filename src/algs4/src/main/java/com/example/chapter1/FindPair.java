package com.example.chapter1;

import java.util.Arrays;
import java.util.Random;

public class FindPair
{
    static class Result
    {
        int start = -1;
        int end = -1;
        double value = 0;

        Result(double v) {
            value = v;
        }
    }

    // https://leetcode.com/problems/minimum-absolute-difference/
    // Exercise 1.4.16 暴力循环 O(N^2)
    public static void closestPair(double[] arr, Result r) {
        for (int i = 0; i < arr.length; i++) {
            for (int j = i + 1; j < arr.length; j++) {
                double v = Math.abs(arr[i] - arr[j]);
                if (v < r.value) {
                    r.start = i;
                    r.end = j;
                    r.value = v;
                }
            }
        }
    }

    // 先排序，再遍历
    public static void closestPair2(double[] arr, Result r) {
        Arrays.sort(arr); // quick sort, N * log(N)
        for (int i = 1; i < arr.length - 1; i++) {
            double v = arr[i] - arr[i-1];
            if (v < r.value) {
                r.start = i;
                r.end = i + 1;
                r.value = v;
            }
        }
    }

    // Exercise 1.4.17 暴力循环 O(N^2)
    public static void farthestPair(double[] arr, Result r) {
        for (int i = 0; i < arr.length; i++) {
            for (int j = i + 1; j < arr.length; j++) {
                double v = Math.abs(arr[i] - arr[j]);
                if (v > r.value) {
                    r.start = i;
                    r.end = j;
                    r.value = v;
                }
            }
        }
    }

    // 找出最大和最小的两个值，他们就是最遥远的一对, O(N)
    public static void farthestPair2(double[] arr, Result r) {
        double min = Double.MAX_VALUE;
        double max = Double.MIN_VALUE;

        for (int i = 0; i < arr.length; i++) {
            if (arr[i] > max){
                max = arr[i];
                r.end = i;
            }
            if (arr[i] < min) {
                min = arr[i];
                r.start = i;
            }
        }
    }

    public static void printResult(double[] arr, Result r)
    {
        r.value = Math.abs(arr[r.start] - arr[r.end]);
        System.out.println(String.format("%d(%f), %d(%f), %f", r.start, arr[r.start], r.end, arr[r.end], r.value));
    }

    public static void main(String[] args)
    {
        double[] arr = {
                59.26, 27.70, 52.43, 4.84, 50.84, 46.17, 66.80, 85.52, 85.60, 34.44,
                75.99, 76.56, 7.84, 49.87, 12.84, 10.39, 34.40, 22.37, 59.53, 3.42,
                74.39, 69.93, 10.33, 13.48, 25.07, 3.99, 9.40, 38.34, 23.80, 61.98,
                77.62, 70.04, 10.55, 79.50, 89.81, 44.47, 72.98, 12.27, 21.71, 0.20,
                89.11, 25.04, 4.21, 90.87, 35.95, 38.69, 24.93, 71.50, 7.04, 21.20,
                10.65, 98.17, 39.82, 94.83, 31.36, 72.81, 35.40, 30.80, 50.58, 18.06,
                97.85, 1.54, 31.41, 52.78, 96.20, 44.00, 10.30, 34.43, 77.99, 59.12,
                97.78, 52.69, 34.04, 83.64, 27.59, 67.92, 27.31, 38.84, 82.76, 40.19,
                77.51, 3.79, 2.03, 50.70, 12.71, 12.97, 47.42, 68.52, 11.45, 20.64,
                66.96, 13.55, 16.66, 58.55, 56.27, 43.85, 85.95, 86.47, 33.43, 93.82,
        };

        Result r1 = new Result(Double.MAX_VALUE);
        Result r2 = new Result(Double.MIN_VALUE);
        closestPair2(arr, r1);
        printResult(arr, r1);

        farthestPair2(arr, r2);
        printResult(arr, r2);
    }
}