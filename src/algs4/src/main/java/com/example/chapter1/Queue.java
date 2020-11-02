package com.example.chapter1;

import java.util.Iterator;

class Queue<E> implements Iterable<E>
{
    // 双链表
    private class Node
    {
        Node next;
        E item;
    }

    private Node first;
    private Node last;
    private int N;

    public boolean isEmpty() {
        return first == null;
    }

    public int size() {
        return N;
    }

    public void enqueue(E item)
    {
        Node node = new Node();
        node.item = item;
        node.next = null;
        if (first == null) {
            first = node;
        } else {
            last.next = node;
            last = node;
        }
        N++;
    }

    public E dequeue()
    {
        E item = first.item;
        first = first.next;
        if (first == null) {
            last = null;
        }
        N--;
        return item;
    }

    public Iterator<E> iterator() {
        return new QueueIterator();
    }

    private class QueueIterator implements Iterator<E> {
        private Node node = first;
        public boolean hasNext() { return node != null; }
        public E next() { return node.item; }
        public void remove() { /* do nothing */ }
    }
}