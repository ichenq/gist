package com.example.chapter2;

import edu.princeton.cs.algs4.*;

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

    public static double timeRandomInput(String algo, int N, int T)
    {
        double total = 0.0;
        Double[] arr = new Double[N];
        for (int t = 0; t < T; t++)
        {
            for (int i = 0; i < N; i++) {
                arr[i] = StdRandom.uniform();
            }
            total += time(algo, arr);
        }
        return total;
    }

    public static void main(String[] args)
    {
        String algo1 = args[0];
        String algo2 = args[1];
        int N = Integer.parseInt(args[2]);
        int T = Integer.parseInt(args[3]);
        double t1 = timeRandomInput(algo1, N, T);
        double t2 = timeRandomInput(algo2, N, T);
        StdOut.printf("for %d random Doubles\n", N);
        StdOut.printf("%s(%.2f) is %.2f times faster than %s(%.2f)\n", algo1, t1, t2/t1, algo2, t2);
    }
}