package com.example.chapter2;

public class RingBuffer<E> {
    private E[] buffer = null;
    private int reader = 0;
    private int writer = 0;

    public RingBuffer(int n)
    {
        buffer = (E[])new Object[n];
    }

    public void put(E e)
    {
        writer = (writer + 1) % buffer.length;
        buffer[writer - 1] = e;
    }

    public E get()
    {
        if (reader >= writer)
            return null;
        reader = (reader + 1) % buffer.length;
        return buffer[reader - 1];
    }

    public int size()
    {
        return Math.abs(writer - reader);
    }

    //
    public int capacity()
    {
        return buffer.length - size();
    }


}
