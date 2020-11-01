package com.example.chapter2;

import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.util.Objects;

public class FixedCapacityStack<E>
{
    private E[] array;
    private int n = 0;

    public FixedCapacityStack(int n)
    {
        this.array = (E[])new Object[n];
    }

    public boolean isEmpty() {
        return n == 0;
    }

    public int size() {
        return n;
    }

    public void push(E e)
    {
        this.array[this.n++ % this.n] = e;
    }

    public E pop() {
        this.n--;
        E e = this.array[this.n];
        this.array[this.n] = null;
        return e;
    }

    public static void main(String[] args)
    {
        FixedCapacityStack<String> stack = new FixedCapacityStack<>(100);
        while (!StdIn.isEmpty())
        {
            String item = StdIn.readString();
            if (!item.equals("-"))
                stack.push(item);
            else if (!stack.isEmpty()) {
                StdOut.print(stack.pop() + " ");
            }
        }
        StdOut.println(String.format("(%d left on stack)", stack.size()));
    }
}