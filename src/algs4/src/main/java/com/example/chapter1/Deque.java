package com.example.chapter1;

import java.util.*;

// Exercise 1.3.33 实现双向队列
public class Deque<E> implements Iterable<E>
{
    class LinkedNode
    {
        LinkedNode next;
        LinkedNode prev;
        E value;
    }

    private LinkedNode first;
    private LinkedNode last;
    private int n = 0;

    public Deque()
    {
    }

    public boolean isEmpty() {
        return n == 0;
    }

    public int size() {
        return n;
    }

    public void pushLeft(E e)
    {
        LinkedNode node = new LinkedNode();
        node.value = e;
        if (first == null) {
            first = node;
            last = node;
        } else {
            node.next = first;
            first.prev = node;
            first = node;
        }
        n++;
    }

    public void pushRight(E e)
    {
        LinkedNode node = new LinkedNode();
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

    public E popLeft()
    {
        LinkedNode node = first;
        n--;
        if (n == 0) {
            first = null;
            last = null;
        } else {
            LinkedNode next = first.next;
            node.next = null;
            next.prev = null;
            first = next;
        }
        return node.value;
    }

    public E popRight()
    {
        LinkedNode node = last;
        n--;
        if (n == 0) {
            last = null;
            first = null;
        } else {
            LinkedNode prev = node.prev;
            prev.next = null;
            node.prev = null;
            last = prev;
        }
        return node.value;
    }

    // Exercise 1.3.47 可连接的队列
    public void catenate(Deque<E> other)
    {
        while (!other.isEmpty()) {
            E v = other.popLeft();
            pushRight(v);
        }
    }

    public Iterator<E> iterator() {
        return new DequeIterator();
    }

    private class DequeIterator implements Iterator<E> {
        private LinkedNode node = first;
        public boolean hasNext() { return node != null; }
        public E next() {
            LinkedNode cur = node;
            node = node.next;
            return cur.value;
        }
        public void remove() { /* do nothing */ }
    }

    public static void main(String args[])
    {
        Deque<Integer> deq = new Deque<>();
        for (int i = 0; i < 10; i++) {
            deq.pushLeft(i);
        }
        for (int i = 10; i < 20; i++) {
            deq.pushRight(i);
        }
        for (int n : deq)
        {
            System.out.println(n);
        }
        System.out.println("---------");
        for (int i = 0; i < 10; i++) {
            int n = deq.popLeft();
            System.out.println(n);
        }
        for (int i = 0; i < 10; i++) {
            int n = deq.popRight();
            System.out.println(n);
        }
    }
}
