#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// 二分迭代法
double my_sqrt1(double x)
{
    if (x <= 0)
    {
        return -1.0;
    }
    double low = 0;
    double high = x;
    double mid = (low + high) / 2;
    while ((high - mid) > FLT_EPSILON)
    {
        if (mid * mid > x) {
            high = mid;
        }
        else {
            low = mid;
        }
        mid = (high + low) / 2;
    }
    return mid;
}

// 求出根号a的近似值, 首先随便猜一个近似值x，然后不断令x等于x和a/x的平均数
double my_sqrt2(double a)
{
    if (a <= 0)
        return -1.0;
    double x = a / 2;
    double last = 0;
    while (x != last)
    {
        last = x;
        x = (last + a / x) / 2;
    }
    return newg;
}

void test_sqrt()
{
    double arr[] = { 2, 3, 4, 5, 9 };
    for (int i = 0; i < _countof(arr); i++)
    {
        double f1 = my_sqrt1(arr[i]);
        double f2 = my_sqrt2(arr[i]);
        double f3 = sqrt(arr[i]);
        printf("%f, %f, %f\n", f1, f2, f3);
    }
}