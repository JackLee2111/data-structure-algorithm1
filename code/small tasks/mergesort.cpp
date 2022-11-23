//归并排序及求逆序对
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdio.h>
using namespace std;
#define N 1000005
int a[N], b[N]; // b为辅助数组
long long cnt;
void merge_sort(int l, int r)
{
    // 如果整个区间中元素个数大于1，则继续分割
    if (r - l > 0)
    {
        // 1、尽量划分为数量相等的2个子序列，并排序
        int mid = (l + r) / 2;
        merge_sort(l, mid);
        merge_sort(mid + 1, r);

        // printf("%d-%d  %d-%d\n",p,mid ,q ,r);

        // 2、将2个有序的序列合并成一个有序序列
        int i = l;                 // 辅助数组的下标
        int p = l, q = mid + 1;    // p、q分别指向2个序列的头部
        while (p <= mid || q <= r) // 左右两部分只要有一部分不为空，需要继续合并
        {
            if (q > r || (p <= mid && a[p] <= a[q])) // 从左半数组复制到辅助数组
                b[i++] = a[p++];
            else
            {
                b[i++] = a[q++];
                cnt += mid - p + 1; // 将逆序对的个数累加起来，f(q) = mid - p + 1
            }
        }

        // 3、将辅助数组b中排好序的元素复制到a中
        for (i = l; i <= r; i++)
            a[i] = b[i];
    }
}
int main()
{
    int n;
    // FILE *fp;
    // fp = fopen("input.txt", "r");
    //  fscanf()
    while (cin >> n)
    {
        for (int i = 1; i <= n; i++)
            cin >> a[i];
        cnt = 0;
        merge_sort(1, n);
        /*
        for (int i = 1; i <= n; i++)
            cout << a[i] << " ";
        cout << endl;
        */
        cout << "逆序对有：" << cnt << endl;
    }

    // fclose(fp);
    return 0;
}