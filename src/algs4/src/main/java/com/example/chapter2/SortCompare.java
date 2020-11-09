package com.example.chapter2;

import edu.princeton.cs.algs4.*;

import java.util.Arrays;


public class SortCompare
{
    public static double time(String algo, Double[] arr)
    {
        Stopwatch timer = new Stopwatch();
        if (algo.equals("Bubble")) {
            Bubble.sort(arr);
        }
        else if (algo.equals("Insertion")) {
            Insertion.sort(arr);
        }
        else if (algo.equals("Selection")) {
            Selection.sort(arr);
        }
        else if (algo.equals("Shell")) {
            Shell.sort(arr);
        }
        return timer.elapsedTime();
    }

    //随机数组排序
    public static double timeRandomInput(String algo, int N, int T)
    {
        double total = 0.0;
        Double[] arr = new Double[N];
        for (int t = 0; t < T; t++)
        {
            for (int i = 0; i < N; i++) {
                arr[i] = StdRandom.uniform() * 100;
            }
            total += time(algo, arr);
        }
        return total;
    }

    // 逆序数组排序
    public static double timeReverseInput(String algo, int N, int T)
    {
        double total = 0.0;
        Double[] arr = new Double[N];
        for (int t = 0; t < T; t++)
        {
            for (int i = 0; i < N; i++) {
                arr[i] = StdRandom.uniform() * 100.0;
            }
            Arrays.sort(arr, (Double a, Double b) -> a - b < 0 ? 1 : -1);
            total += time(algo, arr);
        }
        return total;
    }

    // 数组内容都一样
    public static double timeEqualInput(String algo, int N, int T)
    {
        double total = 0.0;
        Double[] arr = new Double[N];
        for (int t = 0; t < T; t++)
        {
            for (int i = 0; i < N; i++) {
                arr[i] = 1.0;
            }
            total += time(algo, arr);
        }
        return total;
    }

    public static Double[] randomArray(int N)
    {
        Double[] arr = new Double[N];
        for (int i = 0; i < N; i++) {
            arr[i] = StdRandom.uniform() * 100.0;
        }
        return arr;
    }

    public static void benchRandomInput(String algo1, String algo2, int N, int T) {
        double t1 = timeRandomInput(algo1, N, T);
        double t2 = timeRandomInput(algo2, N, T);
        StdOut.printf("for %d random Doubles\n", N);
        StdOut.printf("%s(%.2f) is %.2f times faster than %s(%.2f)\n", algo1, t1, t2/t1, algo2, t2);
    }

    // Exercise 2.1.7
    public static void benchReverseInput(String algo1, String algo2, int N, int T) {
        double t1 = timeReverseInput(algo1, N, T);
        double t2 = timeReverseInput(algo2, N, T);
        StdOut.printf("for %d reverse Doubles\n", N);
        StdOut.printf("%s(%.2f) is %.2f times faster than %s(%.2f)\n", algo1, t1, t2/t1, algo2, t2);
    }

    // Exercise 2.1.6
    public static void benchEqualInput(String algo1, String algo2, int N, int T) {
        double t1 = timeEqualInput(algo1, N, T);
        double t2 = timeEqualInput(algo2, N, T);
        StdOut.printf("for %d equal Doubles\n", N);
        StdOut.printf("%s(%.2f) is %.2f times faster than %s(%.2f)\n", algo1, t1, t2/t1, algo2, t2);
    }

    // 相同随机数组排序
    public static void benchRandomSameInput(String algo1, String algo2, int N, int T) {
        Double[][] arrs = new Double[T][];
        for (int i = 0; i < T; i++) {
            arrs[i] = randomArray(N);
        }
        double t1 = 0;
        double t2 = 0;
        for (int i = 0; i < T; i++)
        {
            Double[] arr = arrs[i];
            Double[] arr2 = Arrays.copyOf(arr, arr.length);
            t1 += time(algo1, arr);
            t2 += time(algo2, arr2);
        }
        StdOut.printf("for %d same input Doubles\n", N);
        StdOut.printf("%s(%.2f) is %.2f times faster than %s(%.2f)\n", algo1, t1, t2/t1, algo2, t2);
    }

    public static void main(String[] args)
    {
        String algo1 = args[0];
        String algo2 = args[1];
        int N = Integer.parseInt(args[2]);
        int T = Integer.parseInt(args[3]);
        //benchRandomInput(algo1, algo2, N, T);
        benchRandomSameInput(algo1, algo2, N, T);
        //benchReverseInput(algo1, algo2, N, T);
        //benchEqualInput(algo1, algo2, N, T);
    }
}