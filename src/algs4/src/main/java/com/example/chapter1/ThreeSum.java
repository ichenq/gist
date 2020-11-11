package com.example.chapter1;

import java.util.*;


public class ThreeSum
{
    static class Item implements Comparable<Item>
    {
        int first = 0;
        int second = 0;
        int third = 0;

        public Item(int a, int b, int c) {
            int[] arr = {a, b, c};
            Arrays.sort(arr);
            first = arr[0];
            second = arr[1];
            third = arr[2];
        }

        @Override
        public int compareTo(Item other) {
            int v = Integer.compare(first, other.first);
            if (v == 0) {
                v = Integer.compare(second, other.second);
                if (v == 0) {
                    return Integer.compare(third, other.third);
                }
            }
            return v;
        }
    }

    // 暴力循环，时间复杂度O(N^3)，空间复杂度O(1)
    public static void sum(int[] a, int target, Set<Item> set)
    {
        for (int i = 0; i < a.length; i++) {
            for (int j = i + 1; j < a.length; j++) {
                for (int k = j + 1; k < a.length; k++) {
                    if ((a[i] + a[j] + a[k]) == target) {
                        set.add(new Item(a[i], a[j], a[k]));
                    }
                }
            }
        }
    }

    // Exercise 1.4.15 快速3-sum
    public static void fast_sum(int[] a, int target, Set<Item> set)
    {
        Map<Integer, Integer> map = new HashMap<>();
        for (int i = 0; i < a.length; i++) {
            map.put(a[i], i);
        }
        for (int i = 0; i < a.length; i++) {
            for (int j = i + 1; j < a.length; j++) {
                int v = target - a[i] - a[j];
                Integer k = map.get(v);
                if (k != null && k != i && k != j) {
                    set.add(new Item(a[i], a[j], a[k]));
                }
            }
        }
    }

    public static void sum3(int[] a, int target, Set<Item> set)
    {
        Arrays.sort(a); // quicksort N*ln(N)
        for (int i = 0; i < a.length; i++) {
            for (int j = i + 1; j < a.length; j++) {
                int v = target - a[i] - a[j];
                int idx = BinarySearch.search(v, a);
                if (idx >= 0 && idx > j) {
                    set.add(new Item(a[i], a[j], a[idx]));
                }
            }
        }
    }

    public static void printSet(Set<Item> v) {
        Map<Integer, ArrayList<Item>> map = new TreeMap<>();
        for (Item item : v)
        {
            if (!map.containsKey(item.first)) {
                map.put(item.first, new ArrayList<>());
            }
            ArrayList<Item> arr = map.get(item.first);
            arr.add(item);
        }
        for (Map.Entry<Integer, ArrayList<Item>> entry : map.entrySet()) {
            ArrayList<Item> items = entry.getValue();
            System.out.println(String.format("%d #%d:", entry.getKey(), items.size()));
            System.out.print('\t');
            for (Item item : items)
            {
                System.out.print(String.format("(%d,%d,%d), ", item.first, item.second, item.third));
            }
            System.out.println("");
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
        ThreeSum.sum(arr, target, set1);
        ThreeSum.fast_sum(arr, target, set2);
        ThreeSum.sum3(arr, target, set3);
        System.out.println("method1 index: #" + set1.size());
        printSet(set1);
        System.out.println("method2 index: #" + set2.size());
        printSet(set2);
        System.out.println("method3 index: #" + set3.size());
        printSet(set3);

        assert(set1.equals(set2));
        assert(set3.equals(set2));
    }
}