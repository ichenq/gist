package com.example.chapter2;

import edu.princeton.cs.algs4.*;

// merge sort
public class Merge
{
    public static void sort(Comparable[] arr)
    {
        Comparable[] aux = new Comparable[arr.length];
        //sort_td(arr, aux, 0, arr.length - 1);
        sort_bu(arr, aux);
    }

    // top-down sort
    public static void sort_td(Comparable[] arr,Comparable[] aux, int lo, int hi)
    {
        if (lo >= hi)
            return;
        int mid = lo + (hi - lo) / 2;
        StdOut.printf("start sort : [%d %d]\n", lo, hi);
        sort_td(arr, aux, lo, mid);
        sort_td(arr, aux, mid + 1, hi);
        merge2(arr, aux, lo, mid, hi);
        StdOut.printf("start merge: [%d %d]\n", lo, hi);
    }

    // bottom-up sort
    public static void sort_bu(Comparable[] arr, Comparable[] aux)
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

    public static void merge(Comparable[] arr, Comparable[] aux, int lo, int mid, int hi)
    {
        for (int i = 0; i <= hi; i++) {
            aux[i] = arr[i];
        }
        int i = lo;
        int j = mid + 1;
        for (int k = lo; k <= hi; k++) {
            if (i > mid) {
                arr[k] = aux[j++];
            }
            else if (j > hi) {
                arr[k] = aux[i++];
            }
            else if (aux[j].compareTo(aux[i]) < 0) {
                arr[k] = aux[j++];
            }
            else {
                arr[k] = aux[i++];
            }
        }
    }

    public static void merge2(Comparable[] arr, Comparable[] aux, int lo, int mid, int hi)
    {
        int k = 0;
        int i = lo, j = mid + 1;
        while (i <= mid && j <= hi)
        {
            if (arr[i].compareTo(arr[j]) <= 0) {
                aux[k++] = arr[i++];
            } else{
                aux[k++] = arr[j++];
            }
        }
        // rest of left half
        while (i <= mid) {
            aux[k++] = arr[i++];
        }
        // rest of right half
        while (j <= hi) {
            aux[k++] = arr[j++];
        }
        // copy back
        for (i = 0; i < k; i++) {
            arr[lo+i] = aux[i];
        }
    }

    public static void main(String[] args)
    {
        final Integer[] arr = SortUtil.randomIntArray(39);

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
        Merge.sort(arr);
        assert SortUtil.isSorted(arr);
    }
}