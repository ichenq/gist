package com.example.chapter1;

// 队列实现的栈
public class QueueBasedStack<E>
{
    private Queue<E>  queue;

    public QueueBasedStack()
    {
        queue = new Queue<>();
    }

    public int size() {
        return queue.size();
    }

    public boolean isEmpty() {
        return size() == 0;
    }

    // 把所有旧的数据项取出，并逐个压入队列
    public void push(E e)
    {
        int cursize = queue.size();
        queue.enqueue(e);
        for (int i = 0; i < cursize; i++) {
            E v = queue.dequeue();
            queue.enqueue(v);
        }
    }

    public E pop()
    {
        return queue.dequeue();
    }
}