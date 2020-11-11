package com.example.chapter1;

import java.util.Iterator;

public class ResizingArrayStack<E> implements Iterable<E>
{
    private E[] a;
    private int N = 0;

    public ResizingArrayStack()
    {
        resize(1);
    }

    public boolean isEmpty() { return N == 0; }
    public int size() { return N; }

    private void resize(int max) {
        E[] temp = (E[]) new Object[max];
        for (int i = 0; i < this.N; i++) {
            temp[i] = this.a[i];
        }
        this.a = temp;
    }

    public void push(E e) {
        if (this.N == this.a.length) {
            resize(this.N * 2);
        }
        this.a[this.N++] = e;
    }

    // Exercise 1.3.7
    public E peek() {
        return this.a[this.N - 1];
    }

    public E pop() {
        this.N--;
        E e = this.a[this.N];
        this.a[this.N] = null;
        if (this.N > 0 && this.N == this.a.length/4) {
            resize(this.a.length / 2); // shrink
        }
        return e;
    }

    // Exercise 1.3.47 可连接的栈
    public void catenate(ResizingArrayStack<E> other)
    {
        while (!other.isEmpty()) {
            E v = other.pop();
            push(v);
        }
    }

    public Iterator<E> iterator() {
        return new ReverseArrayIterator();
    }

    private class ReverseArrayIterator implements Iterator<E> {
        private int i = N;
        public boolean hasNext() { return i > 0; }
        public E next() { return a[--i]; }
        public void remove() { /* do nothing */ }
    }
}