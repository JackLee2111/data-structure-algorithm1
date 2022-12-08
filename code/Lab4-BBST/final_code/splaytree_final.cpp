#include <iostream>
using namespace std;
const int N = 100006;
int splay_n, splay_m, op, root, Splay_Node_cnt;
struct Splay_Node
{
    int f, c, v, s, rev, ch[2];
} t[N];

int check(int x)
{
    return t[t[x].f].ch[1] == x;
}
void push_up(int x)
{
    t[x].s = t[t[x].ch[0]].s + t[t[x].ch[1]].s + t[x].c;
}

void push_down(int x)
{
    if (t[x].rev)
    {
        swap(t[x].ch[0], t[x].ch[1]);
        t[x].rev = 0;
        t[t[x].ch[0]].rev ^= 1, t[t[x].ch[1]].rev ^= 1;
    }
    return;
}

//旋转操作
void Rotate(int x)
{
    int y = t[x].f, z = t[y].f, k = check(x), v = t[x].ch[k ^ 1];
    t[y].ch[k] = v, t[v].f = y, t[z].ch[check(y)] = x, t[x].f = z;
    t[x].ch[k ^ 1] = y, t[y].f = x;
    push_up(y);
    push_up(x);
}

void Splay(int x, int goal = 0)
{
    while (t[x].f != goal)
    {
        if ((t[t[x].f].f) != goal)
        {
            if (check(x) == check(t[x].f))
                Rotate(t[x].f);
            else
                Rotate(x);
        }
        Rotate(x);
    }
    if (!goal)
        root = x;
}
//查找
void find(int x)
{
    if (!root)
        return;
    int pos = root;
    while (t[pos].ch[x > t[pos].v] && x != t[pos].v)
    {
        pos = t[pos].ch[x > t[pos].v];
    }
    Splay(pos);
    return;
}
//插入
void Insert(int x)
{
    int pos = root, fa = 0;
    while (pos && t[pos].v != x)
    {
        fa = pos, pos = t[pos].ch[x > t[pos].v];
    }
    pos = ++Splay_Node_cnt;
    if (fa)
        t[fa].ch[x > t[fa].v] = pos;
    t[pos].ch[1] = t[pos].ch[0] = 0;
    t[pos].v = x, t[pos].f = fa, t[pos].s = t[pos].c = 1;
    Splay(pos);
    return;
}

int Kth(int x)
{
    int pos = root;
    while (1)
    {
        push_down(pos);
        if (x <= t[t[pos].ch[0]].s)
            pos = t[pos].ch[0];
        else if (x > t[t[pos].ch[0]].s + t[pos].c)
        {
            x -= t[t[pos].ch[0]].s + 1;
            pos = t[pos].ch[1];
        }
        else
            return pos;
    }
}

//区间翻转
void reverse(int l, int r)
{
    l = Kth(l), r = Kth(r + 2);
    Splay(l);
    Splay(r, l);
    t[t[t[root].ch[1]].ch[0]].rev ^= 1;
    return;
}

//对树遍历输出
void output(int x)
{
    push_down(x);
    if (t[x].ch[0])
        output(t[x].ch[0]);
    if (t[x].v > 1 && t[x].v < splay_n + 2)
        cout << t[x].v - 1 << " ";
    if (t[x].ch[1])
        output(t[x].ch[1]);
}
int main()
{
    cin >> splay_n >> splay_m;
    int x, y;
    for (int i = 1; i <= splay_n + 2; i++)
        Insert(i);
    for (int i = 1; i <= splay_m; i++)
    {
        cin >> x >> y;
        reverse(x, y);
    }
    output(root);
    cout << endl;
    return 0;
}