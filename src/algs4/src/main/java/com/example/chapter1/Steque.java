package com.example.chapter1;

// Exercise 1.3.32 以栈为目标的队列
public class Steque<E>
{
    static class LinkedNode<E>
    {
        LinkedNode<E> next;
        LinkedNode<E> prev;
        E value;
    }

    private LinkedNode<E> first;
    private LinkedNode<E> last;
    private int n = 0;

    public Steque()
    {
    }

    public boolean isEmpty()
    {
        return n == 0;
    }

    public int size()
    {
        return n;
    }

    public void push(E e)
    {
        LinkedNode<E> node = new LinkedNode<>();
        node.value = e;
        if (first == null) {
            first = node;
        } else {
            last.next = node;
            node.prev = last;
        }
        last = node;
        n++;
    }

    public E pop()
    {
        LinkedNode<E> node = last;
        n--;
        if (n == 0) {
            last = null;
            first = null;
        } else {
            LinkedNode<E> prev = node.prev;
            prev.next = null;
            node.prev = null;
            last = prev;
        }
        return node.value;
    }

    public void enqueue(E e)
    {
        push(e);
    }

    public E dequeue()
    {
        LinkedNode<E> node = first;
        n--;
        if (n == 0) {
            first = null;
            last = null;
        } else {
            LinkedNode<E> next = first.next;
            node.next = null;
            next.prev = null;
            first = next;
        }
        return node.value;
    }

    public static void main(String args[])
    {
        Steque<Integer> steq = new Steque<>();
        for (int i = 0; i < 10; i++) {
            steq.push(i);
        }
        System.out.println(steq.size());
        for (int i = 0; i < 5; i++) {
            int n = steq.pop();
            System.out.println(n);
        }
        for (int i = 0; i < 5; i++) {
            int n = steq.dequeue();
            System.out.println(n);
        }
        System.out.println(steq.size());
    }
}
