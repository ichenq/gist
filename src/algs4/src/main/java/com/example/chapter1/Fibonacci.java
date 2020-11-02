package com.example.chapter1;

import java.util.*;
import java.math.BigInteger;

// Exercise 1.1.19
public class Fibonacci
{
    private static BigInteger big1 = new BigInteger("1");
    private static BigInteger big2 = new BigInteger("2");

    static BigInteger fib_slow(BigInteger n)
    {
        int N = n.intValue();
        if(N == 1 || N == 2)
            return big1;
        BigInteger a = fib_slow(n.subtract(big1));
        BigInteger b = fib_slow(n.subtract(big2));
        return a.add(b);
    }

    static BigInteger fib_helper(BigInteger[] arr, BigInteger n)
    {
        int N = n.intValue();
        if(N == 1 || N == 2)
            return big1;
        if (arr[N] != null)
            return arr[N];
        BigInteger a = fib_helper(arr, n.subtract(big1));
        BigInteger b = fib_helper(arr, n.subtract(big2));
        arr[N] = a.add(b);
        return arr[N];
    }

    static BigInteger fib_dp(BigInteger n)
    {
        int N = n.intValue();
        BigInteger[] arr = new BigInteger[N +1];
        return fib_helper(arr, n);
    }

    static BigInteger fib_fast(BigInteger n)
    {
        int N = n.intValue();
        if(N == 1 || N == 2)
            return big1;
        BigInteger n1 = big1;
        BigInteger n2 = big1;
        for (int i = 0; i < N; i++)
        {
            BigInteger sum = n1.add(n2);
            n2 = n1;
            n1 = sum;
        }
        return n1;
    }

    public static void main(String[] args)
    {
        BigInteger r = fib_fast(new BigInteger("100"));
        System.out.println(r);
    }
}