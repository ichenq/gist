package com.example.chapter1;

import java.util.Objects;

// Exercise 1.3.38
public class GeneralizedQueue<E>
{
    private E a[];
    private int N = 0;

    public GeneralizedQueue()
    {
        resize(1);
    }

    public boolean isEmpty() {
        return N == 0;
    }

    public int size() {
        return N;
    }

    public void resize(int max)
    {
        E[] arr = (E[])new Objects[max];
        for (int i = 0; i < N; i++)
        {
            arr[i] = a[i];
        }
        a = arr;
    }

    public void insert(E e) {
        if (N >= a.length) {
            resize(N * 2);
        }
        a[N++] = e;
    }

    public E delete(int k) {
        E value = a[k-1];
        for (int i = k-1; i < N-2; i++)
        {
            a[i] = a[i+1];
        }
        return value;
    }
}