//归并排序求逆序对
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
#define N 10000001
int a[N], b[N]; // b是辅助
int ans;
void mergesort(int l, int r)
{
    // 如果整个区间中元素个数大于1，则继续分割
    if (r - l > 0)
    {
        // 递归
        int mid = (l + r) >> 1;
        mergesort(l, mid);
        mergesort(mid + 1, r);

        // 合并
        int i = l;
        int p = l, q = mid + 1;
        while (p <= mid || q <= r)
        {
            if (q > r || (p <= mid && a[p] <= a[q])) // 从左半数组复制到辅助数组
                b[i++] = a[p++];
            else
            {
                b[i++] = a[q++];
                ans = ans + mid - p + 1; // 将逆序对的个数累加起来
            }
        }

        // 将b中排好序的元素复制到a中
        for (i = l; i <= r; i++)
            a[i] = b[i];
    }
}
int main()
{
    FILE *fp;
    char ch;
    int x;
    int i = 1;
    // 文件读入
    fp = fopen("input.txt", "r");

    // 文件指针
    if (fp == NULL)
    {
        printf("Open filefailure!");
        exit(1);
    }
    else
    {
        while (fscanf(fp, "%d", &x) != EOF)
        {
            fscanf(fp, "%c", &ch);
            a[i] = x;
            i++;
            // printf("%d\n", x);
        }
    }
    //归并求逆序对
    mergesort(1, i - 1);
    cout << "The number of inversion pairs is:" << ans << endl;
    fclose(fp);
    return 0;
}