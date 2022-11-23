#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <queue>
#include <vector>
#include <iostream>
#include <queue>
#include <string>
using namespace std;
const int inf = 0x3f3f3f3f;
const int maxn = 2e4 + 10;
int d[maxn], n, m, vis[maxn]; // d为当前节点到起点的最小距离（已更新），vis为是否结束访问
int s, t;
struct dis
{
    int id, d;
    dis(int id, int d) : id(id), d(d) {}
    bool operator<(const dis &a) const
    {
        return d > a.d;
    }
};

priority_queue<dis> q;

struct node
{
    int to, val, next;
} e[maxn]; //链式向前星存图

int head[maxn], cnt = 0;
void add(int s, int E, int val)
{
    e[++cnt] = {E, val, head[s]};
    head[s] = cnt;
}

int dij(int start)
{
    q.push(dis(start, 0)); //起点入队，距离为0
    d[start] = 0;          //最小距离数组为0

    while (!q.empty()) //优先队列非空
    {
        dis a = q.top(); //取出最小点
        q.pop();

        int now = a.id, minval = a.d; //最小点的id和距离
        vis[now] = 1;                 //取出来设定为已经访问

        for (int i = head[now]; i != -1; i = e[i].next)
        {
            int to = e[i].to, val = e[i].val;
            if (!vis[to] && minval + val < d[to]) //未访问且可更新
            {
                d[to] = minval + val;   //更新最短路数组
                q.push(dis(to, d[to])); //入队
            }
        }
    }
    if (d[t] != inf)
        return d[t]; //起点到终点的最短路径
    else
        return -1;
}

void init()
{
    for (int i = 1; i <= n; i++)
        d[i] = inf;
    memset(head, -1, sizeof(head));
    memset(e, 0, sizeof(e));
    memset(vis, 0, sizeof(vis));
    while (!q.empty())
        q.pop();
    cnt = 0;
}

int main()
{
    while (scanf("%d%d", &n, &m) != EOF)
    {
        init();
        for (int i = 1; i <= m; i++)
        {
            cout << "iii" << i << endl;
            int a, b, c;
            scanf("%d%d%d", &a, &b, &c);
            a++;
            b++;
            add(a, b, c);
            add(b, a, c);
        }
        scanf("%d%d", &s, &t);
        s++;
        t++;
        printf("%d\n", dij(s));
    }
    int j = 0;
    for (int j = 0; j <= n; j++)
    {
        int a, b, c;
        cin >> a >> b >> c;
    }
    return 0;
}
