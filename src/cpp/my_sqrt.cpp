#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// ���ֵ�����
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

// �������a�Ľ���ֵ, ��������һ������ֵx��Ȼ�󲻶���x����x��a/x��ƽ����
double my_sqrt2(double x)
{
    if (x <= 0)
        return -1.0;
    double newg = x / 2;
    double lastg = 0;
    while (newg != lastg)
    {
        lastg = newg;
        newg = (lastg + x / lastg) / 2;
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