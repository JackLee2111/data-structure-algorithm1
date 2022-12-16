// c++去重
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
    int n;
    cin >> n;
    int a[n];
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    sort(a, a + n);               // 将数组从小到大排序
    int m = unique(a, a + n) - a; // unique函数只能去除相邻重复的元素
    for (int i = 0; i < m; i++)
    {
        cout << a[i] << " ";
    }
    return 0;
}
/* unique等同于
 for(int i=1;i<n;i++)
    {
        if(a[i]!=a[i-1]) cout<<a[i]<<" ";
    }
*/