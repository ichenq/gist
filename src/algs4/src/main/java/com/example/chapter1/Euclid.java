package com.example.chapter1;


class Euclid {

    // Exercise 1.1.24
    public static int euclid(int p, int q)
    {
        while(q > 0) {
            System.out.printf("p = %d, q = %d\n", p, q);
            int r = p % q;
            p = q;
            q = r;
        }
        return p;
    }

    // Exercise 1.1.18
    public static int mystery(int a, int b)
    {
        if (b == 0) return 0;
        if (b % 2 == 0) return mystery(a+a, b/2);
        return a + mystery(a+a, b/2);
    }

    public static void main(String[] args) {
        int r = euclid(111111, 1234567);
        System.out.println("euclid: " + r);
        int a = mystery(3, 11);
        System.out.println("mystery: " + a);
    }
}