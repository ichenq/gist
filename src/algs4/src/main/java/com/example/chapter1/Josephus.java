package com.example.chapter1;


public class Josephus {

    class LinkedNode
    {
        LinkedNode next;
        int value = 0;
    }

    // f(1, 1) => 1
    // f(n, m) => (f(n-1, m) + m - 1) % n
    public void fastMethod(int N, int M)
    {
        if (N == 0)
            return;
        int r = 0;
        for (int i = 2; i <= N; i++)
        {
            r = (r + M) % i;
            System.out.println("Kill " + r);
        }
        System.out.println("last alive is " + (r + 1));
    }

    public void listMethod(int N, int M)
    {
        if (N == 0)
            return ;
        LinkedNode list = new LinkedNode();
        list.value = 0;
        LinkedNode head = list;
        for (int i = 1; i < N; i++)
        {
            LinkedNode node = new LinkedNode();
            node.value = i;
            head.next = node;
            head = node;
        }
        head.next = list;   // make circle

        int count = N;
        LinkedNode node = list;
        LinkedNode prev = null;
        while (count > 1)
        {
            for (int i = 0; i < M; i++)
            {
                prev = node;
                node = node.next;
            }
            // delete node
            System.out.println("kill " + node.value);
            prev.next = node.next;
            count--;
        }
        System.out.println("last alive is " + prev.value);
    }

    public static void arrayMethod(int N, int M)
    {
        // TODO:
    }

    public static void main(String[] args)
    {
        Josephus problem = new Josephus();
        System.out.println("fast method:");
        problem.fastMethod(7, 2);
        System.out.println("list method:");
        problem.listMethod(7, 2);
    }
}
