package com.example.chapter1;

import java.util.*;

public class TwoSum
{
    // 暴力循环，时间复杂度O(N^2)，空间复杂度O(1)
    public static Map<Integer, Integer> sum(int[] a, int target)
    {
        Map<Integer, Integer> index = new TreeMap<>();
        int count = 0;
        for (int i = 0; i < a.length; i++) {
            for (int j = i + 1; j < a.length; j++) {
                if (a[i] + a[j] == target) {
                    if (!index.containsKey(a[j])) {
                        index.put(a[i], a[j]);
                    }
                }
            }
        }
        return index;
    }

    // 使用hashmap, 时间和空间复杂度都为O(N)
    public static Map<Integer, Integer> sum2(int[] a, int target)
    {
        Map<Integer, Integer> index = new TreeMap<>();
        Map<Integer, Integer> map = new HashMap<>();
        for (int i = 0; i < a.length; i++) {
            map.put(a[i], i);
        }
        for (int i = 0; i < a.length; i++) {
            int v = target - a[i];
            Integer j = map.get(v);
            if (j != null && j != i) {
                if (!index.containsKey(a[j])) {
                    index.put(a[i], a[j]);
                }
            }
        }
        return index;
    }

    //
    public static Map<Integer, Integer> sum3(int[] a, int target)
    {
        Map<Integer, Integer> index = new TreeMap<>();
        Arrays.sort(a); // quicksort N*ln(N)
        for (int i = 0; i < a.length; i++) {
            int v = target - a[i];
            int idx = BinarySearch.search(v, a);
            if (idx >= 0 && idx > i) {
                if (!index.containsKey(a[idx])) {
                    index.put(a[i], a[idx]);
                }
            }
        }
        return index;
    }

    public static void printMap(Map<Integer, Integer> v) {
        for (Map.Entry<Integer, Integer> entry : v.entrySet()) {
            int idx1 = entry.getKey();
            int idx2 = entry.getValue();
            System.out.println(String.format("%d --> %d", idx1, idx2));
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
        Map<Integer, Integer> v1 = TwoSum.sum(arr, target);
        Map<Integer, Integer> v2 = TwoSum.sum2(arr, target);
        Map<Integer, Integer> v3 = TwoSum.sum3(arr, target);
        System.out.println("calcute twosum of " + target);
        System.out.println("method1 index: #" + v1.size());
        printMap(v1);
        System.out.println("method2 index: #" + v2.size());
        printMap(v2);
        System.out.println("method3 index: #" + v3.size());
        printMap(v3);
        assert(v1.equals(v2));
        assert(v2.equals(v3));
    }
}