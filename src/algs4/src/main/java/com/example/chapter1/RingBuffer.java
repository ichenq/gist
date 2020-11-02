package com.example.chapter1;

import java.util.Iterator;

public class RingBuffer<E> implements Iterable<E>
{
    private E[] buf = null;
    private int rd = 0;     // read position
    private int wr = 0;     // write position
    private int N = 0;

    public RingBuffer(int n)
    {
        buf = (E[])new Object[n];
    }

    public void put(E e)
    {
        wr = wr % buf.length;
        buf[wr++] = e;
        N = Math.min(N+1, buf.length);
    }

    public E get()
    {
        rd = rd % buf.length;
        N = Math.max(0, N - 1);
        return buf[rd++];
    }

    public int size()
    {
        return N;
    }

    //
    public int capacity()
    {
        return buf.length - N;
    }

    @Override
    public Iterator<E> iterator() {
        return new RingBufferIterator();
    }

    private class RingBufferIterator implements Iterator<E> {
        private int n = 0;
        public boolean hasNext() { return N > 0 && n != N; }
        public E next() { return buf[n++]; }
        public void remove() { /* do nothing */ }
    }

    public static void main(String args[])
    {
        RingBuffer<Integer> buf = new RingBuffer(5);
        for (int i = 0; i < 10; i++)
        {
            buf.put(i);
        }
        for (int n : buf)
        {
            System.out.println(n);
        }
    }
}
