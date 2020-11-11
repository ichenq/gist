package com.example.chapter1;

import java.util.*;
import java.math.*;

public class TwoSum
{
    static class Item implements Comparable<Item>
    {
        int first = 0;
        int second = 0;

        public Item(int a, int b) {
            first = Math.min(a, b);
            second = Math.max(a, b);
        }

        @Override
        public int compareTo(Item other) {
            int v = Integer.compare(first, other.first);
            if (v == 0) {
                return Integer.compare(second, other.second);
            }
            return v;
        }
    }

    // 暴力循环，时间复杂度O(N^2)，空间复杂度O(1)
    public static void sum(int[] a, int target, Set<Item> set)
    {
        for (int i = 0; i < a.length; i++) {
            for (int j = i + 1; j < a.length; j++) {
                if (a[i] + a[j] == target) {
                    set.add(new Item(a[i], a[j]));
                }
            }
        }
    }

    // 使用hashmap, 时间和空间复杂度都为O(N)
    public static void fast_sum(int[] a, int target, Set<Item> set)
    {
        Map<Integer, Integer> map = new HashMap<>();
        for (int i = 0; i < a.length; i++) {
            map.put(a[i], i);
        }
        for (int i = 0; i < a.length; i++) {
            int v = target - a[i];
            Integer j = map.get(v);
            if (j != null && j != i) {
                set.add(new Item(a[i], a[j]));
            }
        }
    }

    //排序+二分查找， O(N*logN)
    public static void sum3(int[] a, int target, Set<Item> set)
    {
        Arrays.sort(a); // quicksort N*ln(N)
        for (int i = 0; i < a.length; i++) {
            int v = target - a[i];
            int idx = BinarySearch.search(v, a);
            if (idx >= 0 && idx > i) {
                set.add(new Item(a[i], a[idx]));
            }
        }
    }

    public static void printSet(Set<Item> v) {
        for (Item item : v) {
            System.out.println(String.format("%d --> %d", item.first, item.second));
        }
    }

    public static void main(String[] args)
    {
        final int[] arr = {
            127,63,169,135,26,12,29,188,66,191,
            81,175,156,12,137,184,147,89,156,115,
            182,115,181,64,38,23,138,198,54,71,
            82,76,179,148,196,12,91,131,34,12,
            2,69,196,65,23,54,145,74,5,74,
            177,198,77,7,165,36,185,35,0,141,
            83,3,63,152,124,108,35,34,81,34,
            64,143,16,115,85,41,136,7,180,20,
            198,199,148,68,164,102,33,44,76,40,
            118,84,162,30,88,118,31,35,171,197,
        };

        int target = 100;
        Set<Item> set1 = new TreeSet<>();
        Set<Item> set2 = new TreeSet<>();
        Set<Item> set3 = new TreeSet<>();
        TwoSum.sum(arr, target, set1);
        TwoSum.fast_sum(arr, target, set2);
        TwoSum.sum3(arr, target, set3);
        System.out.println("calcute twosum of " + target);
        System.out.println("method1 index: #" + set1.size());
        printSet(set1);
        System.out.println("method2 index: #" + set2.size());
        printSet(set2);
        System.out.println("method3 index: #" + set3.size());
        printSet(set3);
        assert(set1.equals(set2));
        assert(set2.equals(set3));
    }
}