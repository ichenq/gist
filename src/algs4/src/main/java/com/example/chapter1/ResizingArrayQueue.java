package com.example.chapter1;

// Exercise 1.3.14 动态数组实现的队列
public class ResizingArrayQueue<E>
{
    private E []a = (E[])new Object[1];
    private int N = 0;
    private int head = 0;
    private int tail = 0;

    ResizingArrayQueue()
    {
    }

    private void resize(int max)
    {
        E[] temp = (E[]) new Object[max];
        for (int i = 0; i < this.N; i++) {
            temp[i] = this.a[i];
        }
        this.a = temp;
        this.head = 0;
        this.tail = N;
    }

    public int size() { return N; }

    public boolean isEmpty() {
        return N == 0;
    }

    public void enqueue(E e)
    {
        if (N >= a.length) {
            resize(N * 2);
        }
        a[tail] = e;
        tail = (tail + 1) % N;
        N++;
    }

    public E dequeue()
    {
        E e = a[head];
        head = (head + 1) % N;
        if (N > 0 && N == a.length/4) {
            resize(a.length / 2); // shrink
        }
        return e;
    }
}