#include <iostream>
#include <vector>
#include <string>
#include <queue>
using namespace std;
// Spaly Tree
const int N = 100006;
int splay_n, splay_m, op, root, Splay_Node_cnt;

// RB Tree
#define RED 0   // 红
#define BLACK 1 // 黑
template <typename T, typename V>
class RBtree
{
private:
    class Node
    {
    private:
        pair<T, V> p;
        int s;
        bool color; // 0红色 1黑色
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
    void LRotate(Node *x) // 左旋操作
    {
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
    Node *minimum(Node *x) // 找最小
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
    Node *maximum(Node *x) // 找最大
    {
        if (x == nil)
            return x;
        while (Rt(x) != nil)
        {
            x = Rt(x);
        }
        return x;
    }
    Node *Lt_node(Node *x) // 前驱节点
    {
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
    Node *Rt_node(Node *x) // 后继节点
    {
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
    Node *find_pre_node(T key) // 找前驱（小于x的最大值）
    {
        Node *parent = nil;
        Node *current = root;
        Node *t = nil;
        while (current != nil)
        { // 当当前节点不为空时
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
    Node *find_next_node(T key) // 找后继（大于x的最小值）
    {
        Node *parent = nil;
        Node *current = root;
        Node *t = nil;
        while (current != nil) // 当前节点不为空时
        {
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
    // 插入操作
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
    // 调整
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
    // 删除操作
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
    // 删除操作
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
    // 调整
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

    // 获取第k大的值
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
    // 找某个x的rank值
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

// AVL tree 实现__________________________________________________________________________
int num; // 中序遍历计数
int num_ans;
template <typename T>
struct AVLTreeNode
{
    T avl_key;      // 关键字
    int avl_height; // 高度
    int size;
    AVLTreeNode *avl_leftChild;  // 左孩子
    AVLTreeNode *avl_rightChild; // 右孩子
    AVLTreeNode *avl_parent;     // 父节点
    AVLTreeNode(T value, AVLTreeNode *l, AVLTreeNode *r, AVLTreeNode *p) : avl_key(value), avl_height(0), avl_leftChild(l), avl_rightChild(r), avl_parent(p) {}
};

template <typename T>
class AVLTree
{
public:
    AVLTree() : avl_root(NULL) {}
    ~AVLTree() { destroy(avl_root); }
    // int &Getsize() { return size; }
    //  中序遍历
    void inOrder() { inOrder(avl_root); }

    // 树的高度
    int height() { return height(avl_root); }

    // 查找AVL树种值为key的结点（递归）
    AVLTreeNode<T> *search(T key) { return search(avl_root, key); }

    // 查找AVL树种值为key的结点（非递归）
    AVLTreeNode<T> *iterativeSearch(T key) { return iterativeSearch(avl_root, key); }

    // 查找最小结点，返回最小结点的键值
    T minimum()
    {
        AVLTreeNode<T> *p = minimum(avl_root);
        if (p != NULL)
            return p->avl_key;
        return (T)NULL;
    }

    // 查找最大结点，返回最大结点的键值
    T maximum()
    {
        AVLTreeNode<T> *p = maximum(avl_root);
        if (p != NULL)
            return p->avl_key;
        return (T)NULL;
    }

    // 将键值为key的结点插入到AVL中
    void insert(T key) { insert(avl_root, key); }

    // 删除键值为key的结点
    void remove(T key)
    {
        AVLTreeNode<T> *z = search(avl_root, key);
        if (z != NULL)
            avl_root = remove(avl_root, z);
    }

    // 销毁AVL树
    void destroy() { destroy(avl_root); }

    // 以下为函数的具体实现
    // 中序遍历
    void inOrder(AVLTreeNode<T> *node)
    {
        if (node == NULL)
            return;
        inOrder(node->avl_leftChild);
        cout << node->avl_key << " ";
        inOrder(node->avl_rightChild);
    }

    // 查询排名为x的数，中序遍历————————————
    void getTthNum(int x)
    {
        num = 0;
        inOrder2(avl_root, x);
    }
    void inOrder2(AVLTreeNode<T> *node, int x)
    {
        if (node == NULL)
            return;
        inOrder2(node->avl_leftChild, x);
        num++;
        if (num == x)
            cout << node->avl_key << endl;
        inOrder2(node->avl_rightChild, x);
    }

    // 查询数x的排名，中序遍历——————————————
    void find_rank(T x)
    {
        num = 0;
        num_ans = 9999999;
        inOrder3(avl_root, x);
        cout << num_ans << endl;
    }
    void inOrder3(AVLTreeNode<T> *node, int x)
    {
        if (node == NULL)
            return;
        inOrder3(node->avl_leftChild, x);
        num++;
        if (node->avl_key == x && num_ans > num)
            num_ans = num;
        inOrder3(node->avl_rightChild, x);
    }

    // 更新父节点
    void update_parent(AVLTreeNode<T> *node)
    {
        queue<AVLTreeNode<T> *> Q; // 辅助队列
        Q.push(node);              // 根节点入队
        node->avl_parent = nullptr;
        while (!Q.empty()) // 在队列再次变空之前，反复迭代
        {
            AVLTreeNode<T> *x = Q.front();
            Q.pop();
            if (x->avl_leftChild)
            {
                x->avl_leftChild->avl_parent = x;
                Q.push(x->avl_leftChild); // 左孩子入队
            }
            if (x->avl_rightChild)
            {
                x->avl_rightChild->avl_parent = x;
                Q.push(x->avl_rightChild); // 右孩子入队
            }
        }
    }

    // 找到大于key的最小值
    void find_next(T key)
    {
        AVLTreeNode<T> *temp = loose_search(this->avl_root, key); // 大于key的最小值

        if (temp->avl_key > key)
        {
            cout << temp->avl_key << endl;
            return;
        }
        update_parent(this->avl_root);
        AVLTreeNode<T> *ans = find_next_node(temp, key);

        while (ans->avl_key <= key) // 找到大于key的最小值
        {
            AVLTreeNode<T> *ans1 = find_next_node(ans, key);
            ans = ans1;
        }
        cout << ans->avl_key << endl;
    }

    // 值为key的直接后继
    AVLTreeNode<T> *find_next_node(AVLTreeNode<T> *temp, T key) // 定位直接后继
    {
        AVLTreeNode<T> *s = temp; // 记录后继的临时变量
        if (temp->avl_rightChild) // 若有右孩子，则直接后继必在右子树中，具体地就是
        {
            s = temp->avl_rightChild; // 右子树中
            while (s->avl_leftChild)
                s = s->avl_leftChild; // 最靠左（最小）的节点
        }
        else // 否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
        {
            while (s->avl_parent->avl_rightChild == s)
                s = s->avl_parent; // 逆向地沿右向分支，不断朝左上方移动
            s = s->avl_parent;     // 最后再朝右上方移动一步，即抵达直接后继（如果存在）
        }
        return s;
    }

    // 找到小于key的最大值
    void find_pre(T key)
    {
        AVLTreeNode<T> *temp = loose_search(this->avl_root, key);
        if (temp->avl_key < key)
        {
            cout << temp->avl_key << endl;
            return;
        }
        update_parent(this->avl_root);
        AVLTreeNode<T> *ans = find_pre_node(temp, key);

        while (ans->avl_key >= key) // 找到大于key的最小值
        {
            AVLTreeNode<T> *ans1 = find_pre_node(ans, key);
            ans = ans1;
        }
        cout << ans->avl_key << endl;
    }

    // 值为key的直接前驱
    AVLTreeNode<T> *find_pre_node(AVLTreeNode<T> *temp, T key) // 定位直接前驱
    {
        AVLTreeNode<T> *s = temp; // 记录后继的临时变量
        if (temp->avl_leftChild)  // 若有左孩子，则直接后继必在左子树中，具体地就是
        {
            s = temp->avl_leftChild; // 左子树中
            while (s->avl_rightChild)
                s = s->avl_rightChild; // 最靠右（最大）的节点
        }
        else // 否则，直接前驱应是“将当前节点包含于其右子树中的最低祖先”，具体地就是
        {
            while (s->avl_parent->avl_leftChild == s)
                s = s->avl_parent; // 逆向地沿左向分支，不断朝右上方移动
            s = s->avl_parent;     // 最后再朝左上方移动一步，即抵达直接前驱（如果存在）
        }
        return s;
    }

    // 高度
    int height(AVLTreeNode<T> *node)
    {
        return node != NULL ? node->avl_height : 0;
    }

    void destroy(AVLTreeNode<T> *&tree)
    {
        if (tree == NULL)
            return;
        if (tree->avl_leftChild != NULL)
            destroy(tree->avl_leftChild);
        if (tree->avl_rightChild != NULL)
            destroy(tree->avl_rightChild);
        delete tree;
    }

    // 寻找二叉树中的值为key的节点，返回该点的指针
    AVLTreeNode<T> *search(AVLTreeNode<T> *node, T key)
    {
        if (node == NULL || node->avl_key == key)
            return node;
        if (key < node->avl_key)
            return search(node->avl_leftChild, key);
        else
            return search(node->avl_rightChild, key);
    }

    // 寻找二叉树中的值为key的节点，若找不到则返回最后一个不是null的节点
    AVLTreeNode<T> *loose_search(AVLTreeNode<T> *node, T key)
    {
        if (node == NULL || node->avl_key == key)
            return node;
        else if (key < node->avl_key && node->avl_leftChild)
            return loose_search(node->avl_leftChild, key);
        else if (key < node->avl_key && (!node->avl_leftChild))
            return node;
        else if (key > node->avl_key && (!node->avl_rightChild))
            return loose_search(node->avl_rightChild, key);
        else if (key > node->avl_key && (!node->avl_rightChild))
            return node;
    }

    /*
    //寻找二叉树中的值为key最靠左的节点，返回该点的指针
    AVLTreeNode<T> *search_leftest(AVLTreeNode<T> *node, T key)
    {
        if (node == NULL || node->avl_key == key)
        {
            if (node->avl_leftChild->avl_key < key)
                return node;
            else
                return search(node->avl_leftChild, key);
        }

        if (key < node->avl_key)
            return search(node->avl_leftChild, key);
        else
            return search(node->avl_rightChild, key);
    }

    //寻找二叉树中的值为key最靠右的节点，返回该点的指针
    AVLTreeNode<T> *search_rightest(AVLTreeNode<T> *node, T key)
    {
        if (node == NULL || node->avl_key == key)
        {
            if (node->avl_rightChild->avl_key > key)
                return node;
            else
                return search(node->avl_rightChild, key);
        }
        if (key < node->avl_key)
            return search(node->avl_leftChild, key);
        else
            return search(node->avl_rightChild, key);
    }
    */

    AVLTreeNode<T> *iterativeSearch(AVLTreeNode<T> *node, T key)
    {
        while ((node != NULL) && (node->avl_key != key))
        {
            if (key < node->avl_key)
                node = node->avl_leftChild;
            else
                node = node->avl_rightChild;
        }
        return node;
    }

    AVLTreeNode<T> *minimum(AVLTreeNode<T> *node)
    {
        if (node == NULL)
            return NULL;
        while (node->avl_leftChild != NULL)
        {
            node = node->avl_leftChild;
        }
        return node;
    }

    AVLTreeNode<T> *maximum(AVLTreeNode<T> *node)
    {
        if (node == NULL)
            return NULL;
        while (node->avl_rightChild != NULL)
        {
            node = node->avl_rightChild;
        }
        return node;
    }

    AVLTreeNode<T> *insert(AVLTreeNode<T> *&node, T key)
    {
        if (node == NULL)
        {
            node = new AVLTreeNode<T>(key, NULL, NULL, NULL);
        }
        else if (key <= node->avl_key) // key插入node的左子树的情况
        {
            node->avl_leftChild = insert(node->avl_leftChild, key);
            // 插入节点后，如果AVL树失衡，需要进行相应调节
            if (height(node->avl_leftChild) - height(node->avl_rightChild) == 2)
            {
                if (key < node->avl_leftChild->avl_key)
                    node = leftLeftRotation(node);
                else
                    node = leftRightRotation(node);
            }
        }
        else if (key > node->avl_key) // key插入node的右子树的情况
        {
            node->avl_rightChild = insert(node->avl_rightChild, key);
            // 插入节点后，如果AVL树失衡，需要进行相应调节
            if (height(node->avl_rightChild) - height(node->avl_leftChild) == 2)
            {
                if (key > node->avl_rightChild->avl_key)
                    node = rightRightRotation(node);
                else
                    node = rightLeftRotation(node);
            }
        }
        /*
        else
        {
            cout << "添加失败，不能添加相同的结点" << endl;
        }*/
        node->avl_height = max(height(node->avl_leftChild), height(node->avl_rightChild)) + 1;
        return node;
    }

    AVLTreeNode<T> *remove(AVLTreeNode<T> *&node, AVLTreeNode<T> *z)
    {
        if (node == NULL || z == NULL)
            return NULL;

        if (z->avl_key < node->avl_key) // 待删除的节点在tree的左子树中
        {
            node->avl_leftChild = remove(node->avl_leftChild, z);
            // 删除节点后，如果AVL树失衡，需要进行相应调节
            if (height(node->avl_rightChild) - height(node->avl_leftChild) == 2)
            {
                AVLTreeNode<T> *r = node->avl_rightChild;
                if (height(r->avl_leftChild) > height(r->avl_rightChild))
                    node = rightLeftRotation(node);
                else
                    node = rightRightRotation(node);
            }
        }
        else if (z->avl_key > node->avl_key) // 待删除的节点在node的右子树中
        {
            // 删除节点后，如果AVL树失衡，需要进行相应调节
            node->avl_rightChild = remove(node->avl_rightChild, z);
            if (height(node->avl_leftChild) - height(node->avl_rightChild) == 2)
            {
                AVLTreeNode<T> *l = node->avl_leftChild;
                if (height(l->avl_rightChild) > height(l->avl_leftChild))
                    node = leftRightRotation(node);
                else
                    node = leftLeftRotation(node);
            }
        }
        else // 当前node就是要删除的节点
        {
            if ((node->avl_leftChild != NULL) && (node->avl_rightChild != NULL))
            {
                if (height(node->avl_leftChild) > height(node->avl_rightChild))
                {
                    if (height(node->avl_leftChild) > height(node->avl_rightChild))
                    {
                        /* 如果node节点的左子树比右子树高，则:
                         * (01)找出node的左子树中最大节点
                         * (02)将该最大节点的值赋值给node
                         * (03)删除该最大节点
                         * 相当于用node的左子树中最大节点作为node的替身
                         * 这种方式删除node左子树中最大节点之后，AVL树任然是平衡的
                         */
                        AVLTreeNode<T> *max = maximum(node->avl_leftChild);
                        node->avl_key = max->avl_key;
                        node->avl_leftChild = remove(node->avl_leftChild, max);
                    }
                    else
                    {
                        /* 如果node节点的右子树比左子树高，则:
                         * (01)找出node的右子树中最小节点
                         * (02)将该最小节点的值赋值给node
                         * (03)删除该最小节点
                         * 相当于用node的右子树中最小节点作为node的替身
                         * 这种方式删除node右子树中最小节点之后，AVL树任然是平衡的
                         */
                        AVLTreeNode<T> *min = minimum(node->avl_rightChild);
                        node->avl_key = min->avl_key;
                        node->avl_rightChild = remove(node->avl_rightChild, min);
                    }
                }
            }
            else
            {
                // 被删除的节点等于node，并且node有一个孩子
                // 将当前节点指向该孩子节点并删除当前节点
                AVLTreeNode<T> *tmp = node;
                node = node->avl_leftChild != NULL ? node->avl_leftChild : node->avl_rightChild;
                delete tmp;
            }
        }
        return node;
    }

private:
    /* LL：左子树的左边失去平衡(左单旋转)
     */

    AVLTreeNode<T> *leftLeftRotation(AVLTreeNode<T> *&k2)
    {
        AVLTreeNode<T> *k1 = k2->avl_leftChild;
        k2->avl_leftChild = k1->avl_rightChild;
        k1->avl_rightChild = k2;
        k2->avl_height = std::max(height(k2->avl_leftChild), height(k2->avl_rightChild)) + 1;
        k1->avl_height = std::max(height(k1->avl_leftChild), k2->avl_height) + 1;
        return k1;
    }

    /* RR：右子树的右边失去平衡(右单旋转)
     */
    AVLTreeNode<T> *rightRightRotation(AVLTreeNode<T> *&k1)
    {
        AVLTreeNode<T> *k2 = k1->avl_rightChild;
        k1->avl_rightChild = k2->avl_leftChild;
        k2->avl_leftChild = k1;

        k1->avl_height = std::max(height(k1->avl_leftChild), height(k1->avl_rightChild)) + 1;
        k2->avl_height = std::max(height(k2->avl_rightChild), k1->avl_height) + 1;
        return k2;
    }

    /* LR：左子树的右边失去平衡(左双旋转)
     */
    AVLTreeNode<T> *leftRightRotation(AVLTreeNode<T> *&k3)
    {
        k3->avl_leftChild = rightRightRotation(k3->avl_leftChild);
        return leftLeftRotation(k3);
    }

    /* RL：右子树的左边失去平衡(右双旋转)
     */
    AVLTreeNode<T> *rightLeftRotation(AVLTreeNode<T> *&k1)
    {
        k1->avl_rightChild = leftLeftRotation(k1->avl_rightChild);
        return rightRightRotation(k1);
    }

private:
    AVLTreeNode<T> *avl_root; // AVL树的根节点
};

void solve_AVLTree(int n)
{
    AVLTree<int> tree;

    int opt, x;
    for (int i = 0; i < n; i++)
    {
        cin >> opt >> x;
        if (opt == 1) // 插入x
        {
            tree.insert(x);
        }
        else if (opt == 2) // 删除x (若有多个相同的数，删除一个即可)
        {
            tree.remove(x);
        }

        else if (opt == 3) // 查询x的排名(定义为比x小的数的个数 +1)
        {
            tree.find_rank(x);
        }
        else if (opt == 4) // 查询排名为x的数
        {
            tree.getTthNum(x);
        }

        else if (opt == 5) // 求x的前驱
        {
            tree.find_pre(x);
        }

        else if (opt == 6) // 求x的后继
        {
            tree.find_next(x);
        }
    }
}
void solve_RBTree(int n)
{
    RBtree<int, int> *mp = new RBtree<int, int>();
    int opt, x;
    for (int i = 0; i < n; i++)
    {
        cin >> opt >> x;
        if (opt == 1) // 插入x
        {
            mp->Insert({x, 1});
        }
        else if (opt == 2) // 删除x (若有多个相同的数，删除一个即可)
        {
            mp->Delete(x);
        }
        else if (opt == 3) // 查询x的排名(定义为比x小的数的个数 +1)
        {
            cout << mp->find_rank(x, mp->root) << endl;
        }
        else if (opt == 4) // 查询排名为x的数
        {
            cout << mp->get_Tth_Num(x, mp->root)->Getdata() << endl;
        }
        else if (opt == 5) // 求x的前驱(定义为小于x的最大数)
        {
            cout << mp->find_pre_node(x)->Getdata() << endl;
        }
        else if (opt == 6) // 求x的后继(定义为大于x的最小数)
        {
            cout << mp->find_next_node(x)->Getdata() << endl;
        }
    }
}

// 问题2 Splay Tree————————————————————————————————————————————————————————————
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

// 旋转操作
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
// 查找
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
// 插入
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

// 区间翻转
void reverse(int l, int r)
{
    l = Kth(l), r = Kth(r + 2);
    Splay(l);
    Splay(r, l);
    t[t[t[root].ch[1]].ch[0]].rev ^= 1;
    return;
}

// 对树遍历输出
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
// Splay Tree ————————————————————————————————————————————————————————————

int main()
{
    int n, mode, que;
    cout << "请输入需要解决的问题：（1或2）" << endl;
    cin >> que;
    if (que == 1)
    {
        cout << "请输入需要构建的平衡树，1为红黑树，2为AVL树：" << endl;
        cin >> mode;
        cout << "请按样例进行输入：" << endl;
        cin >> n;
        if (mode == 1)
        {
            solve_RBTree(n);
        }
        else if (mode == 2)
        {
            solve_AVLTree(n);
        }
        else
            cout << "illegal input" << endl;
    }
    else if (que == 2)
    {
        cout << "请按样例进行输入：" << endl;
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
    }
    else
        cout << "illegal input" << endl;

    return 0;
}