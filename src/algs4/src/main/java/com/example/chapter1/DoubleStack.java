package com.example.chapter1;

// Exercise 1.3.48 用1个双向队列实现2个栈
public class DoubleStack<E>
{
    private Deque<E> deque = new Deque<>();
    private int N1 = 0;
    private int N2 = 0;

    public DoubleStack()
    {
    }

    public int size() {
        return N1;
    }

    public int size2() {
        return N2;
    }

    public void push(E e) {
        deque.pushRight(e);
        N1++;
    }

    public E pop() {
        N1--;
        return deque.popRight();
    }

    public void push2(E e) {
        deque.pushRight(e);
        N2++;
    }

    public E pop2() {
        N2--;
        return deque.popRight();
    }
}