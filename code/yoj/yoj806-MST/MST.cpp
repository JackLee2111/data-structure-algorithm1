#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

int fa[1000010];
int n, m, need_r;
struct ENode
{
    int from, to, value, color; //起点、终点、权值、颜色
};
ENode e[1000010];

bool cmp(ENode a, ENode b) // sort规则
{
    if (a.value == b.value)
        return a.color < b.color; //边权相同时红色优先
    else
        return a.value < b.value; //边权从小到大排序
}

int find(int x) //并查集，查
{
    if (fa[x] == x)
        return x;
    return fa[x] = find(fa[x]);
}

int sum;       //累计权重和
int ans;       //最终权重和
int temp_red;  //当前红边计数
int cnt_e = 0; //当前引入的边数之和

void kruskal() //最小生成树
{
    sort(e + 1, e + 1 + m, cmp);         // e边从第一个开始
    for (int i = 1; cnt_e != n - 1; i++) //直到贪心地加入了n-1条边
    {
        int x = find(e[i].from); //起点所在集合
        int y = find(e[i].to);
        if (x == y) //判断两端点是否在同一集合内
            continue;
        else if (x != y)
        {
            cnt_e++;
            fa[x] = y; //以终点所在的集合的祖先为祖先
            if (e[i].color == 0)
                temp_red++; //红边数量统计
            sum += e[i].value;
        }
    }
}
void binary_search()
{
    int l = -101, r = 101; //二分，初始值比100大就行
    while (l <= r)
    {
        int mid = (l + r) >> 1; //二分中值
        for (int i = 1; i <= m; i++)
            if (e[i].color == 0) //红边的权重加上中值
                e[i].value += mid;

        for (int i = 1; i <= n + 1; i++) //初始化集合祖先节点为自己
            fa[i] = i;

        sum = 0; //每次最短路径搜索前要清零
        cnt_e = 0;
        temp_red = 0;

        kruskal();

        //红边多于需要的，说明红边权重要加一些，左端点右移到mid+1,；更新ans
        if (temp_red >= need_r)
        {
            l = mid + 1;
            ans = sum - need_r * mid;
        }
        else
            r = mid - 1;

        //每次要把加上的减掉，否则影响下一次循环search
        for (int i = 1; i <= m; i++)
            if (e[i].color == 0)
                e[i].value -= mid;
    }
}
int main()
{
    cin >> n >> m >> need_r;
    for (int i = 1; i <= m; i++)
    {
        cin >> e[i].from >> e[i].to >> e[i].value >> e[i].color;
        e[i].from++;
        e[i].to++; //为了使边从1开始计数
    }

    binary_search();

    cout << ans << '\n';
    return 0;
}