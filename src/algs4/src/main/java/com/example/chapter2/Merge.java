package com.example.chapter2;

import java.util.*;
import edu.princeton.cs.algs4.*;

// 归并排序
public class Merge
{
    public static void sort(Comparable[] arr)
    {
        Comparable[] aux = new Comparable[arr.length];
    }

    public static void sort(Comparable[] arr,Comparable[] aux, int lo, int hi)
    {
        if (lo >= hi) // 0或1个元素，则已排好序
            return;
        int mid = lo + (hi - lo) / 2;
        sort(arr, aux, lo, mid);
        sort(arr, aux, mid + 1, hi);
        merge(arr, aux, lo, mid, hi);
    }

    public static void merge(Comparable[] arr, Comparable[] aux, int lo, int mid, int hi)
    {
        int i = lo;
        int j = mid + 1;
        for (int k = lo; k <= hi; k++) {
            aux[k] = arr[k];
        }
        for (int k = lo; k <= hi; k++) {
            if (i > mid)              arr[k] = aux[j++];
            else if (j > hi)
                arr[k] = aux[i++];

            else if (aux[j].compareTo(aux[i]) < 0)
                arr[k] = aux[j++];

            else
                arr[k] = aux[i++];

        }
    }

    public static void sort2(Comparable[] arr)
    {
        // make a copy
        Comparable[] aux = new Comparable[arr.length];
        for (int i = 0; i < arr.length; i++) {
            aux[i] = arr[i];
        }
        topDownSort(arr, aux, 0, arr.length - 1);
        //bottomUpSort(arr, aux);
    }

    // 自顶向下递归， top-down sort
    public static void topDownSort(Comparable[] arr,Comparable[] aux, int lo, int hi)
    {
        if (lo >= hi) // 0或1个元素，则已排好序
            return;
        int mid = lo + (hi - lo) / 2;
        StdOut.printf("start sort : [%d %d]\n", lo, hi);
        topDownSort(arr, aux, lo, mid);
        topDownSort(arr, aux, mid + 1, hi);
        merge3(arr, aux, lo, mid, hi);
        StdOut.printf("start merge: [%d %d]\n", lo, hi);
    }

    // 自底向上循环，bottom-up sort
    public static void bottomUpSort(Comparable[] arr, Comparable[] aux)
    {
        int N = arr.length;
        for (int sz = 1; sz < N; sz *= 2)
        {
            for (int lo = 0; lo < N - sz; lo += sz*2)
            {
                int hi = Math.min(lo + sz*2 - 1, N-1);
                StdOut.printf("merge range: [%d %d]\n", lo, hi);
                merge(arr, aux, lo, lo+sz-1, hi);
            }
        }
    }


    public static void merge2(Comparable[] arr, Comparable[] aux, int lo, int mid, int hi)
    {
        int i = lo, k = lo, j = mid + 1;
        while (i <= mid && j <= hi)
        {
            if (arr[i].compareTo(arr[j]) <= 0) {
                arr[k++] = aux[i++];
            } else{
                arr[k++] = aux[j++];
            }
        }
        // rest of left half
        while (i <= mid) {
            arr[k++] = aux[i++];
        }
        // rest of right half
        while (j <= hi) {
            arr[k++] = aux[j++];
        }
    }

    public static void merge3(Comparable[] arr, Comparable[] aux, int lo, int mid, int hi)
    {
        int i = lo, k = lo, j = mid;
        for (; k < hi; k++)
        {
            if (i < mid && (j >= hi || arr[i].compareTo(arr[j]) <= 0)) {
                arr[k] = aux[i++];
            } else {
                arr[k] = aux[j++];
            }
        }
    }

    // Exercise 2.2.10 降序归并，可以去掉检测某半边是否用尽，非稳定
    public static void merge_aux(Comparable[] arr, Comparable[] aux, int lo, int mid, int hi)
    {

    }

    // Exercise 2.2.17 对链表排序
    public static LinkedList<Comparable> sortList(LinkedList<Comparable> list)
    {
        int N = list.size();
        if (N <= 1)
            return list;

        LinkedList<Comparable> left = new LinkedList<>();
        LinkedList<Comparable> right = new LinkedList<>();
        int cnt = 0;
        for (Comparable v : list)
        {
            if (cnt < N/2) {
                left.addLast(v);
            } else {
                right.addLast(v);
            }
            cnt++;
        }
        left = sortList(left);
        right = sortList(right);
        return mergeList(left, right);
    }

    // 合并两个有序链表
    public static LinkedList<Comparable> mergeList(LinkedList<Comparable> left, LinkedList<Comparable> right)
    {
        LinkedList<Comparable> result = new LinkedList<>();
        while (!left.isEmpty() && !right.isEmpty())
        {
            Comparable item1 = left.peekFirst();
            Comparable item2 = right.peekFirst();
            // item1 <= item2
            if (item1.compareTo(item2) <= 0) {
                result.addLast(item1);
                left.removeFirst();
            } else {
                result.addLast(item2);
                right.removeFirst();
            }
        }
        while (!left.isEmpty()) {
            result.addLast(left.peekFirst());
            left.removeFirst();
        }
        while (!right.isEmpty()) {
            result.addLast(right.peekFirst());
            right.removeFirst();
        }
        return result;
    }

    public static void testSortArray()
    {
        final Integer[] arr1 = SortUtil.randomIntArray(39);

        Integer[] arr2 = {
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
        Merge.sort(arr1);
        assert SortUtil.isSorted(arr1);
    }

    public static void testSortList()
    {
        Integer[] arr = {
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
        LinkedList<Comparable> list = SortUtil.arrayToList(arr);
        list = Merge.sortList(list);
        for (Comparable v : list)
        {
            StdOut.println(v);
        }
    }

    public static void main(String[] args)
    {
        //testSortArray();
        testSortList();
    }
}