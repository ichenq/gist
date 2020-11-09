package com.example.chapter2;

// insertion sort
public class Insertion
{
    public static void sortInt(int[] arr)
    {
        for (int i = 1; i < arr.length; i++)
        {
            int j = i - 1;
            int curr = arr[i];
            while (j >= 0 && arr[j] > curr) {
                arr[j+1] = arr[j];
                j--;
            }
        }
    }

    public static void sort(Comparable[] arr)
    {
        for (int i = 1; i < arr.length; i++)
        {
            for (int j = i; j > 0; j--)
            {
                if (arr[j-1].compareTo(arr[j]) > 0) {
                    SortUtil.exch(arr, j-1, j);
                }
            }
        }
    }

    public static void main(String[] args)
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
        Insertion.sort(arr);
        assert SortUtil.isSorted(arr);
        for (int n : arr)
        {
            System.out.println(n);
        }
    }
}