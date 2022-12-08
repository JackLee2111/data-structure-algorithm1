#include <iostream>
#include <vector>
#include <string>
using namespace std;
#define RED 0   //红色结点
#define BLACK 1 //黑色结点
template <typename T, typename V>
class RBtree
{
private:
    class Node
    {
    private:
        pair<T, V> p;
        int s;
        bool color; // 0代表红色 1代表黑色
        Node *Lt;
        Node *Rt;
        Node *pre;

    public:
        Node(pair<T, V> _p, int _s, bool _color, Node *_Lt = nullptr, Node *_Rt = nullptr, Node *_pre = nullptr) : p(_p), s(_s), color(_color), Lt(_Lt), Rt(_Rt), pre(_pre) {}
        Node(Node *tmp) : p(tmp->p), s(tmp->s), color(tmp->color), Lt(tmp->Lt), Rt(tmp->Rt), pre(tmp->pre) {}
        Node *&GetLt() { return Lt; }
        Node *&GetRt() { return Rt; }
        Node *&Getpre() { return pre; }
        T &Getdata() { return p.first; }
        V &Getinfo() { return p.second; }
        pair<T, V> *Getpair() { return &p; }
        bool Getcolor() const { return color; }
        int &Getsize() { return s; }
        void SetLt(Node *&tmp) { Lt = tmp; }
        void SetRt(Node *&tmp) { Rt = tmp; }
        void Setpre(Node *&tmp) { pre = tmp; }
        void Setdata(T tmp) { p.first = tmp; }
        void Setinfo(V tmp) { p.second = tmp; }
        void Setcolor(bool tmp) { color = tmp; }
    };

    bool color(Node *node)
    {
        if (node == nil)
            return true;
        else
            return node->Getcolor();
    }
    void Setcolor(Node *node, bool col)
    {
        node->Setcolor(col);
    }
    Node *&pre(Node *node)
    {
        return node->Getpre();
    }
    Node *&Lt(Node *node)
    {
        return node->GetLt();
    }
    Node *&Rt(Node *node)
    {
        return node->GetRt();
    }
    void LRotate(Node *x)
    { //左旋
        Node *tmp = Rt(x);
        x->SetRt(Lt(tmp));
        if (Lt(tmp) != nil)
            Lt(tmp)->Setpre(x);
        tmp->Setpre(pre(x));
        if (pre(x) == nil)
            root = tmp;
        else
        {
            if (Lt(pre(x)) == x)
                pre(x)->SetLt(tmp);
            else
                pre(x)->SetRt(tmp);
        }
        tmp->SetLt(x);
        x->Setpre(tmp);
        x->Getsize() = 1;
        if (x->GetLt() != nil)
            x->Getsize() += x->GetLt()->Getsize();
        if (x->GetRt() != nil)
            x->Getsize() += x->GetRt()->Getsize();
        tmp->Getsize() = x->Getsize() + 1;
        if (tmp->GetRt() != nil)
            tmp->Getsize() += tmp->GetRt()->Getsize();
    }
    void RRotate(Node *x)
    {
        Node *tmp = Lt(x);
        x->SetLt(Rt(tmp));
        if (Rt(tmp) != nil)
            Rt(tmp)->Setpre(x);
        tmp->Setpre(pre(x));
        if (pre(x) == nil)
            root = tmp;
        else
        {
            if (Rt(pre(x)) == x)
                pre(x)->SetRt(tmp);
            else
                pre(x)->SetLt(tmp);
        }
        tmp->SetRt(x);
        x->Setpre(tmp);
        x->Getsize() = 1;
        if (x->GetLt() != nil)
            x->Getsize() += x->GetLt()->Getsize();
        if (x->GetRt() != nil)
            x->Getsize() += x->GetRt()->Getsize();
        tmp->Getsize() = x->Getsize() + 1;
        if (tmp->GetLt() != nil)
            tmp->Getsize() += tmp->GetLt()->Getsize();
    }
    void transplant(Node *u, Node *v)
    {
        if (pre(u) == nil)
            root = v;
        else if (u == Lt(pre(u)))
            pre(u)->SetLt(v);
        else
            pre(u)->SetRt(v);
        v->Setpre(pre(u));
    }
    Node *minimum(Node *x)
    {
        if (x == nil)
            return x;
        while (Lt(x) != nil)
        {
            x->Getsize()--;
            x = Lt(x);
        }
        return x;
    }
    Node *maximum(Node *x)
    {
        if (x == nil)
            return x;
        while (Rt(x) != nil)
        {
            x = Rt(x);
        }
        return x;
    }
    Node *Lt_node(Node *x)
    { //前驱
        Node *tmp = x;
        if (tmp->GetLt() == nil)
        {
            while (tmp->Getpre() != nil && tmp->Getpre()->GetLt() == tmp)
            {
                tmp = tmp->Getpre();
            }
            tmp = tmp->Getpre();
        }
        else
        {
            tmp = tmp->GetLt();
            while (tmp->GetRt() != nil)
            {
                tmp = tmp->GetRt();
            }
        }
        return tmp;
    }
    Node *Rt_node(Node *x)
    { //后继
        Node *tmp = x;
        if (tmp->GetRt() == nil)
        {
            while (tmp->Getpre() != nil && tmp->Getpre()->GetRt() == tmp)
            {
                tmp = tmp->Getpre();
            }
            tmp = tmp->Getpre();
        }
        else
        {
            tmp = tmp->GetRt();
            while (tmp->GetLt() != nil)
            {
                tmp = tmp->GetLt();
            }
        }
        return tmp;
    }

public:
    Node *root;
    Node *nil;
    RBtree()
    {
        nil = new Node({-1, -1}, 0, BLACK, nil, nil, nil);
        root = nil;
    }
    class iterator
    {
    private:
        Node *_node;

    public:
        Node *Getnode() { return _node; }
        iterator &operator++()
        {
            _node = _node->Rt_node();
            return *this;
        }
        iterator &operator--()
        {
            _node = _node->Lt_node();
            return *this;
        }
        iterator &operator=(iterator a)
        {
            _node = a._node;
            return *this;
        }
        friend bool operator==(iterator a, iterator b)
        {
            return a.Getnode() == b.Getnode();
        }
        friend bool operator!=(iterator a, iterator b)
        {
            return a.Getnode() != b.Getnode();
        }
        pair<T, V> *operator->() { return _node->Getpair(); }
        iterator(Node *__node = nullptr) : _node(__node) {}
        iterator(iterator const &iter) : _node(iter._node) {}
    };
    Node *find(Node *x, T key)
    {
        Node *tmp = x;
        while (tmp != nil)
        {
            if (tmp->Getdata() > key)
            {
                tmp = tmp->GetLt();
            }
            else if (tmp->Getdata() < key)
            {
                tmp = tmp->GetRt();
            }
            else
                break;
        }
        return tmp;
    }
    Node *find_pre_node(T key)
    {
        Node *parent = nil;
        Node *current = root;
        Node *t = nil;
        while (current != nil)
        { //当当前节点不为空时
            if (current->Getdata() > key)
            {
                parent = current;
                current = current->GetLt();
            }
            else if (current->Getdata() < key)
            {
                parent = current;
                current = current->GetRt();
            }
            else
            {
                t = Lt_node(current);
                break;
            }
        }
        if (current->Getdata() == key)
        {
            while (t->Getdata() == key)
            {
                t = Lt_node(t);
            }
            return t;
        }
        if (parent->Getdata() > key)
        {
            return Lt_node(parent);
        }
        else
        {
            return parent;
        }
    }
    Node *find_next_node(T key)
    {
        Node *parent = nil;
        Node *current = root;
        Node *t = nil;
        while (current != nil)
        { //当当前节点不为空时
            if (current->Getdata() > key)
            {
                parent = current;
                current = current->GetLt();
            }
            else if (current->Getdata() < key)
            {
                parent = current;
                current = current->GetRt();
            }
            else
            {
                t = Rt_node(current);
                break;
            }
        }
        if (current->Getdata() == key)
        {
            while (t->Getdata() == key)
            {
                t = Rt_node(t);
            }
            return t;
        }
        if (parent->Getdata() < key)
        {
            return Rt_node(parent);
        }
        else
        {
            return parent;
        }
    }
    void Insert(pair<T, V> p)
    {
        Node *y = nil;
        Node *x = root;
        Node *z = new Node(p, 1, RED, nil, nil, nil);
        while (x != nil)
        {
            y = x;
            x->Getsize()++;
            if (p.first < x->Getdata())
                x = Lt(x);
            else
                x = Rt(x);
        }
        z->Setpre(y);
        if (y == nil)
            root = z;
        else if (z->Getdata() < y->Getdata())
        {
            y->SetLt(z);
        }
        else
            y->SetRt(z);
        z->SetLt(nil);
        z->SetRt(nil);
        z->Setcolor(RED);
        FixAfterInsert(z);
    }
    void FixAfterInsert(Node *z)
    {
        while (color(z->Getpre()) == RED)
        {
            if (pre(z) == Lt(pre(pre(z))))
            {
                Node *y = Rt(pre(pre(z)));
                if (color(y) == RED)
                {
                    pre(z)->Setcolor(BLACK);    // case1
                    y->Setcolor(BLACK);         // case1
                    pre(pre(z))->Setcolor(RED); // case1
                    z = pre(pre(z));            // case1
                    continue;
                }
                if (z == Rt(pre(z)))
                {               // case2
                    z = pre(z); // case2
                    LRotate(z); // case2
                }
                pre(z)->Setcolor(BLACK);    // case3
                pre(pre(z))->Setcolor(RED); // case3
                RRotate(pre(pre(z)));       // case3
            }
            else
            {
                Node *y = Lt(pre(pre(z)));
                if (color(y) == RED)
                {
                    pre(z)->Setcolor(BLACK);    // case1
                    y->Setcolor(BLACK);         // case1
                    pre(pre(z))->Setcolor(RED); // case1
                    z = pre(pre(z));            // case1
                    continue;
                }
                if (z == Lt(pre(z)))
                {               // case2
                    z = pre(z); // case2
                    RRotate(z); // case2
                }
                pre(z)->Setcolor(BLACK);    // case3
                pre(pre(z))->Setcolor(RED); // case3
                LRotate(pre(pre(z)));       // case3
            }
        }
        root->Setcolor(BLACK);
    }

    bool Delete(Node *z)
    {
        Node *y = z;
        int yoc = color(y);
        Node *x = z;
        if (Lt(z) == nil)
        {
            x = Rt(z);
            transplant(z, Rt(z));
        }
        else if (Rt(z) == nil)
        {
            x = Lt(z);
            transplant(z, Lt(z));
        }
        else
        {
            y = minimum(Rt(z));
            yoc = color(y);
            x = Rt(y);
            if (pre(y) == z)
            {
                x->Setpre(y);
            }
            else
            {
                transplant(y, Rt(y));
                y->SetRt(Rt(z));
                Rt(y)->Setpre(y);
            }
            transplant(z, y);
            y->Getsize() = z->Getsize();
            y->SetLt(Lt(z));
            Lt(y)->Setpre(y);
            y->Setcolor(color(z));
        }
        delete z;
        if (yoc == BLACK)
        {
            FixAfterDelete(x);
        }
        return true;
    }
    bool Delete(T val)
    {
        Node *tmp = this->root;
        while (tmp != nil)
        {
            tmp->Getsize()--;
            if (tmp->Getdata() > val)
            {
                tmp = tmp->GetLt();
            }
            else if (tmp->Getdata() < val)
            {
                tmp = tmp->GetRt();
            }
            else
                break;
        }
        if (tmp == nil)
        {
            return false;
        }
        else
        {
            return Delete(tmp);
        }
    }
    void FixAfterDelete(Node *x)
    {
        while (x != root && color(x) == BLACK)
        {
            if (Lt(pre(x)) == x)
            {
                Node *w = Rt(pre(x));
                if (color(w) == RED)
                {
                    w->Setcolor(BLACK);    // case1
                    pre(x)->Setcolor(RED); // case1
                    LRotate(pre(x));       // case1
                    w = Rt(pre(x));        // case1
                }
                if (color(Lt(w)) == BLACK && color(Rt(w)) == BLACK)
                {
                    w->Setcolor(RED); // case2
                    x = pre(x);       // case2
                }
                else
                {
                    if (color(Rt(w)) == BLACK)
                    {
                        Lt(w)->Setcolor(BLACK); // case3
                        w->Setcolor(RED);       // case3
                        RRotate(w);             // case3
                        w = Rt(pre(x));         // case3
                    }

                    w->Setcolor(color(pre(x))); // case4
                    pre(x)->Setcolor(BLACK);    // case4
                    Rt(w)->Setcolor(BLACK);     // case4
                    LRotate(pre(x));            // case4
                    x = root;                   // case4
                }
            }
            else
            {
                Node *w = Lt(pre(x));
                if (color(w) == RED)
                {
                    w->Setcolor(BLACK);    // case1
                    pre(x)->Setcolor(RED); // case1
                    RRotate(pre(x));       // case1
                    w = Lt(pre(x));        // case1
                }
                if (color(Lt(w)) == BLACK && color(Rt(w)) == BLACK)
                {
                    w->Setcolor(RED); // case2
                    x = pre(x);       // case2
                }
                else
                {
                    if (color(Lt(w)) == BLACK)
                    {
                        Rt(w)->Setcolor(BLACK); // case3
                        w->Setcolor(RED);       // case3
                        LRotate(w);             // case3
                        w = Lt(pre(x));         // case3
                    }

                    w->Setcolor(color(pre(x))); // case4
                    pre(x)->Setcolor(BLACK);    // case4
                    Lt(w)->Setcolor(BLACK);     // case4
                    RRotate(pre(x));            // case4
                    x = root;                   // case4
                }
            }
        }
        x->Setcolor(BLACK);
    }

    Node *get_Tth_Num(int k, Node *p)
    {
        if (p->GetLt() == nil)
        {
            if (k == 1)
            {
                return p;
            }
            else
            {
                return get_Tth_Num(k - 1, p->GetRt());
            }
        }
        else
        {
            if (p->GetLt()->Getsize() == k - 1)
                return p;
            else if (p->GetLt()->Getsize() >= k)
                return get_Tth_Num(k, p->GetLt());
            else
                return get_Tth_Num(k - p->GetLt()->Getsize() - 1, p->GetRt());
        }
    }
    int find_rank(T v, Node *p)
    {
        if (p == nil)
            return 1;
        else if (p->Getdata() >= v)
            return find_rank(v, p->GetLt());
        else
            return (1 + (p->GetLt() ? p->GetLt()->Getsize() : 0) + find_rank(v, p->GetRt()));
    }

    Node *begin()
    {
        Node *t = root;
        while (t->Lt_node() != nil)
        {
            t = t->Lt_node();
        }
        return t;
    }
    Node *end()
    {
        Node *t = root;
        while (t->Rt_node() != nil)
        {
            t = t->Rt_node();
        }
        return t;
    }
};

void solve_RBTree(int n)
{
    RBtree<int, int> *mp = new RBtree<int, int>();
    int opt, x;
    for (int i = 0; i < n; i++)
    {
        cin >> opt >> x;
        if (opt == 1) //插入x
        {
            mp->Insert({x, 1});
        }
        else if (opt == 2) //删除x (若有多个相同的数，删除一个即可)
        {
            mp->Delete(x);
        }
        else if (opt == 3) //查询x的排名(定义为比x小的数的个数 +1)
        {
            cout << mp->find_rank(x, mp->root) << endl;
        }
        else if (opt == 4) // 查询排名为x的数
        {
            cout << mp->get_Tth_Num(x, mp->root)->Getdata() << endl;
        }
        else if (opt == 5) //求x的前驱(定义为小于x的最大数)
        {
            cout << mp->find_pre_node(x)->Getdata() << endl;
        }
        else if (opt == 6) //求x的后继(定义为大于x的最小数)
        {
            cout << mp->find_next_node(x)->Getdata() << endl;
        }
    }
}

// void solve_AVLtree(int n);

int main()
{
    int n, mode;
    cout << "请输入需要构建的平衡树，1为红黑树，2为AVL树：" << endl;
    cin >> mode;
    cout << "请按照样例进行输入：" << endl;
    cin >> n;
    if (mode == 1)
    {
        solve_RBTree(n);
    }
    else if (mode == 2)
    {
        // solve_AVLtree(n);
    }
    else
        cout << "illegal input" << endl;
    return 0;
}