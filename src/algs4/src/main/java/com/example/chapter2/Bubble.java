package com.example.chapter2;

// 冒泡排序
public class Bubble
{
    public static void sortInt(int[] arr)
    {
        for (int i = arr.length - 1; i > 0; i--)
        {
            boolean has_swap = false;
            for (int j = 0; j < i; j++)
            {
                if (arr[j] > arr[j+1]) {
                    int tmp = arr[j+1];
                    arr[j+1] = arr[j];
                    arr[j] = tmp;
                    has_swap = true;
                }
            }
            if (!has_swap) {
                break;
            }
        }
    }

    public static void sort(Comparable[] arr)
    {
        for (int i = arr.length - 1; i > 0; i--)
        {
            boolean has_swap = false;
            for (int j = 0; j < i; j++)
            {
                if (arr[j].compareTo(arr[j+1]) > 0) {
                    SortUtil.exch(arr, j+1, j);
                    has_swap = true;
                }
            }
            if (!has_swap) {
                break;
            }
        }
    }

    // 递归版本
    public static void sort2(Comparable[] arr)
    {
        sort_recursive(arr, arr.length);
    }

    // recursive bubble sort
    public static void sort_recursive(Comparable[] arr, int n)
    {
        // base case
        if (n == 1) {
            return ;
        }

        // one pass of bubble sort.
        // after this pass, the largest element is bubbled to end.
        for (int i = 0; i < n - 1; i++)
        {
            if (SortUtil.less(arr[i+1], arr[i])) {
                SortUtil.exch(arr, i, i+1);
            }
        }
        // for remaining array
        sort_recursive(arr, n - 1);
    }

    public static void main(String[] args)
    {
        int[] arr = {
            1, 25, 19, 37, 12, 74, 35, 21,
            54, 61, 80, 93, 80, 74, 61, 54,
            37, 35, 25, 21, 19, 12, 1,
        };
        final Integer[] arr2 = {
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
        Bubble.sortInt(arr);
        Bubble.sort(arr2);
        assert SortUtil.isSorted(arr);
        assert SortUtil.isSorted(arr2);
        for (int n : arr)
        {
            System.out.println(n);
        }
    }
}