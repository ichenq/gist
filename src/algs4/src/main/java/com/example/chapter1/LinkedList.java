package com.example.chapter1;

import java.util.List;

public class LinkedList
{
    // 双链表
    static class ListNode
    {
        ListNode next;
        ListNode prev;
        int value;
    }

    // 单链表
    static class LinkedNode
    {
        LinkedNode next;
        int value;
    }

    // 删除所有值为key的节点
    public static int remove(ListNode node, int key)
    {
        int count = 0;
        while (node != null) {
            ListNode next = node.next;
            if (node.value == key) {
                ListNode prev = node.prev;
                if (prev != null) {
                    prev.next = node.next;
                }
                if (next != null) {
                    next.prev = node.prev;
                }
                node.prev = null;
                node.next = null;
                count++;
            }
            node = next;
        }
        return count;
    }

    public static int maxValue(ListNode node)
    {
        if (node == null)
            return 0;
        int biggest = node.value;
        while (node != null) {
            if (biggest < node.value) {
                biggest = node.value;
            }
            node = node.next;
        }
        return biggest;
    }

    public static int maxValueRecursive(ListNode node)
    {
        if (node == null)
            return 0;
        return Integer.max(node.value, maxValueRecursive(node.next));
    }

    // 反转单链表
    public static LinkedNode reverse(LinkedNode node)
    {
        if (node == null)
            return null;
        if (node.next == null)
            return node;

        LinkedNode prev = null;
        while (node != null) {
            LinkedNode next = node.next;
            node.next = prev;
            prev = node;
            node = next;
        }
        return prev;
    }
}