#include <stdio.h>
#include <math.h>
#define Pi 3.1415926535
int main()
{
    double x, e, s = 0, strue;
    int i, j, flag;
    scanf("%lf%lf", &x, &e);
    while (x > 2 * Pi)
    {
        x = x - 2 * Pi; //回到0-2pi的区间内
    }
    // strue = sin(x);
    double a = 1.0, b, t1;
    flag = 1;
    i = 1;
    while (1)
    {
        for (j = i; j >= 1; j--)
        {
            a = a * (x / j); //循环求阶乘
        }
        s = s + a * flag;
        flag = -flag;
        i = i + 2;
        if (e > fabs(a))
            break;
        a = 1.0;
    }
    printf("%.6lf\n", s);

    return 0;
}