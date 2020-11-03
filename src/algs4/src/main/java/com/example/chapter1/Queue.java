package com.example.chapter1;

import java.util.Iterator;

class Queue<E> implements Iterable<E>
{
    // 双链表
    static class Node<E>
    {
        Node<E> next;
        E item;
    }

    private Node<E> first;
    private Node<E> last;
    private int N;

    public boolean isEmpty() {
        return first == null;
    }

    public int size() {
        return N;
    }

    public void enqueue(E item)
    {
        Node<E> node = new Node<>();
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

    // Exercise 1.3.47
    public void catenate(Queue<E> other)
    {
        while (!other.isEmpty()) {
            E v = other.dequeue();
            enqueue(v);
        }
    }

    public Iterator<E> iterator() {
        return new QueueIterator();
    }

    private class QueueIterator implements Iterator<E> {
        private Node<E> node = first;
        public boolean hasNext() { return node != null; }
        public E next() { return node.item; }
        public void remove() { /* do nothing */ }
    }
}