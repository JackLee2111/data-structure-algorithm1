#include <cstdio>
#define read read1<int>()
#define Type template <typename T>
Type inline const T read1()
{
    T m = 0;
    char k = getchar();
    while (('0' > k || k > '9') && (k != '-'))
        k = getchar();
    const bool f = (k == '-' ? 1 : 0);
    if (f)
        k = getchar();
    while ('0' <= k && k <= '9')
        m = (m << 3) + (m << 1) + (k ^ 48), k = getchar();
    return f ? -m : m;
}
Type const T Max(T a, T b) { return a > b ? a : b; }
Type struct AVL
{
    int tot;
    struct node
    {
        node *l, *r;
        T v;
        int h, s, si;
        node(T tv)
        {
            l = r = NULL;
            v = tv;
            h = s = si = 1;
        }
        node()
        {
            l = r = NULL;
            v = 0;
            h = s = si = 1;
        }
    } * Root, f[1000000];
    AVL()
    {
        Root = NULL;
        tot = 0;
    }
    int height(node *now)
    {
        return now ? now->h : 0;
    }
    int size(node *now)
    {
        return now ? now->s : 0;
    }
    node *includefindindex(node *now, int k)
    {
        if (k <= size(now->l))
            return includefindindex(now->l, k);
        if (k > size(now->l) + now->si)
            return includefindindex(now->r, k - (size(now->l) + now->si));
        return now;
    }
    T operator[](int k)
    {
        return includefindindex(Root, k)->v;
    }
    node *find(T want)
    {
        node *tem = Root;
        while (tem && tem->v != want)
            if (tem->v > want)
                tem = tem->l;
            else
                tem = tem->r;
        return tem;
    }
    int findrank(T want)
    {
        node *tem = Root;
        int t = 0;
        while (tem)
        {
            if (tem->v < want)
                t += size(tem->l) + tem->si, tem = tem->r;
            else if (tem->v > want)
                tem = tem->l;
            else
                return t + size(tem->l) + 1;
        }
        return 1;
    }
    node *levo(node *now)
    {
        node *tem = now->r;
        now->r = tem->l;
        tem->l = now;
        now->h = Max(height(now->l), height(now->r)) + 1;
        now->s = size(now->l) + size(now->r) + now->si;
        tem->h = Max(height(tem->l), height(tem->r)) + 1;
        tem->s = size(tem->l) + size(tem->r) + tem->si;
        return tem;
    }
    node *dext(node *now)
    {
        node *tem = now->l;
        now->l = tem->r;
        tem->r = now;
        now->h = Max(height(now->l), height(now->r)) + 1;
        now->s = size(now->l) + size(now->r) + now->si;
        tem->h = Max(height(tem->l), height(tem->r)) + 1;
        tem->s = size(tem->l) + size(tem->r) + tem->si;
        return tem;
    }
    node *lext(node *now)
    {
        now->l = levo(now->l);
        return dext(now);
    }
    node *devo(node *now)
    {
        now->r = dext(now->r);
        return levo(now);
    }
    node *balance(node *now)
    {
        if (height(now->l) == height(now->r) + 2)
            if (height(now->l->l) > height(now->l->r))
                now = dext(now);
            else
                now = lext(now);
        else if (height(now->r) == height(now->l) + 2)
            if (height(now->r->r) > height(now->r->l))
                now = levo(now);
            else
                now = devo(now);
        return now;
    }
    node *findmin(node *now)
    {
        if (!now)
            return NULL;
        while (now->l)
            now = now->l;
        return now;
    }
    node *findmax(node *now)
    {
        if (!now)
            return NULL;
        while (now->r)
            now = now->r;
        return now;
    }
    node *includeinsert(node *now, T need)
    {
        if (!now)
        {
            now = f + (tot++);
            now->v = need;
            return now;
        }
        if (need == now->v)
            ++now->si;
        else if (need < now->v)
            now->l = includeinsert(now->l, need);
        else
            now->r = includeinsert(now->r, need);
        now->h = Max(height(now->l), height(now->r)) + 1;
        now->s = size(now->l) + size(now->r) + now->si;
        return balance(now);
    }
    node *includeremove(node *now, T need)
    {
        if (!now)
            return NULL;
        if (now->v > need)
            now->l = includeremove(now->l, need);
        else if (now->v < need)
            now->r = includeremove(now->r, need);
        else if (now->si > 1)
            --now->si;
        else if (now->l && now->r)
        {
            node *tem = findmin(now->r);
            now->v = tem->v;
            now->si = tem->si;
            tem->si = 1;
            now->r = includeremove(now->r, now->v);
        }
        else if (now->l || now->r)
            now = now->l ? now->l : now->r;
        else
            return now = NULL;
        now->h = Max(height(now->l), height(now->r)) + 1;
        now->s = size(now->l) + size(now->r) + now->si;
        return balance(now);
    }
    node *bigger(T need)
    {
        node *tem = Root, *ans = NULL;
        while (tem)
        {
            if (tem->v > need)
            {
                if (!ans || tem->v < ans->v)
                    ans = tem;
                tem = tem->l;
            }
            else
                tem = tem->r;
        }
        return ans;
    }
    node *lower(T need)
    {
        node *tem = Root, *ans = NULL;
        while (tem)
        {
            if (tem->v < need)
            {
                if (!ans || tem->v > ans->v)
                    ans = tem;
                tem = tem->r;
            }
            else
                tem = tem->l;
        }
        return ans;
    }
    void insert(T need) { Root = includeinsert(Root, need); }
    void remove(T need) { Root = includeremove(Root, need); }
};
AVL<int> tree;
int main()
{
    int s = read;
    while (s--)
        switch (read)
        {
        case 1:
            tree.insert(read);
            break;
        case 2:
            tree.remove(read);
            break;
        case 3:
            printf("%d\n", tree.findrank(read));
            break;
        case 4:
            printf("%d\n", tree[read]);
            break;
        case 5:
            printf("%d\n", tree.lower(read)->v);
            break;
        default:
            printf("%d\n", tree.bigger(read)->v);
        }
    return 0;
}