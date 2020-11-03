package com.example.chapter1;

// Exercise 1.3.49
public class StackBasedQueue<E>
{
    ResizingArrayStack<E> s1 = new ResizingArrayStack<>();
    ResizingArrayStack<E> s2 = new ResizingArrayStack<>();

    public StackBasedQueue()
    {
    }

    public int size() {
        return s1.size() + s2.size();
    }

    public boolean isEmpty() {
        return size() == 0;
    }

    private void shift() {
        while (!s1.isEmpty()) {
            E v = s1.pop();
            s2.push(v);
        }
    }

    public void enqueue(E e) {
        s1.push(e);
    }

    public E dequeue() {
        if (s2.isEmpty()) {
            shift();
        }
        return s2.pop();
    }
}