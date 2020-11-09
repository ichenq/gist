package com.example.chapter2;

// shell sort
public class Shell
{
    public static void sortInt(int[] arr)
    {
        int N = arr.length;
        int gap = 1;
        while (gap < N/3) {
            gap = 3 * gap + 1;    // 1,4,13,40,121
        }

        for (; gap > 0; gap /= 3)
        {
            System.out.println(String.format("--------gap %d-----------", gap));
            for (int i = gap; i < N; i++)
            {
                for (int j = i; j >= gap; j -= gap)
                {
                    System.out.println(String.format("compare and sort index %d <--> %d", j, j-gap));
                    if (arr[j] > arr[j-gap]) {
                        int tmp = arr[j];
                        arr[j] = arr[j-gap];
                        arr[j-gap] = tmp;
                    }
                }
                System.out.println("-------------end sort---------------");
            }
            System.out.println(String.format("-------------end gap %d-------------", gap));
        }
    }

    public static void sort(Comparable[] arr)
    {
        int N = arr.length;
        int h = 1;
        while (h < N/3) {
            h = 3 * h + 1;    // 1,4,13,40,121
        }
        for (; h > 0; h /= 3)
        {
            for (int i = h; i < N; i++)
            {
                for (int j = i; j >= h && SortUtil.less(arr[j], arr[j-h]); j -= h) {
                    SortUtil.exch(arr, j, j-h);
                }
            }
        }
    }

    public static void main(String[] args)
    {
        final int[] arr = {
                1, 25, 19, 37, 12, 74, 35, 21,
                54, 61, 80, 93, 80, 74, 61, 54,
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
        Shell.sortInt(arr);
        Shell.sort(arr2);
        assert SortUtil.isSorted(arr);
        assert SortUtil.isSorted(arr2);
//        for (int n : arr)
//        {
//            System.out.println(n);
//        }
    }
}