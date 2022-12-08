//#pragma once
#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define _CRT_SECURE_NO_WARNINGS 1

//红黑树的颜色
enum Colour
{
    RED,
    BLACK,
};
//红黑树的节点
template <class K, class V>
struct RBTreeNode
{
    RBTreeNode<K, V> *_left;
    RBTreeNode<K, V> *_right;
    RBTreeNode<K, V> *_parent;

    pair<K, V> _kv;
    Colour _col;

    RBTreeNode(const pair<K, V> &kv)
        : _left(nullptr), _right(nullptr), _parent(nullptr), _kv(kv), _col(RED)
    {
    }
};
//红黑树
template <class K, class V>
class RBTree
{
    typedef RBTreeNode<K, V> Node;

public:
    //构造函数
    RBTree()
        : _root(nullptr)
    {
    }

    void _Destory(Node *&root)
    {
        if (root == nullptr)
            return;

        _Destory(root->_left);
        _Destory(root->_right);

        delete root;
        root = nullptr;
    }
    //析构
    ~RBTree()
    {
        _Destory(_root);
    }

    Node *Find(const K &key)
    {
        Node *cur = _root;
        while (cur)
        {
            if (cur->_kv.first > key)
            {
                cur = cur->_left;
            }
            else if (cur->_kv.first < key)
            {
                cur = cur->_right;
            }
            else
            {
                return cur;
            }
        }
        return nullptr;
    }

    //插入节点
    pair<Node *, bool> Insert(const pair<K, V> &kv)
    {
        //空树
        if (_root == nullptr)
        {
            _root = new Node(kv);
            _root->_col = BLACK;
            return make_pair(_root, true);
        }

        //查找位置插入节点
        Node *cur = _root, *parent = _root;
        while (cur)
        {
            if (cur->_kv.first > kv.first)
            {
                parent = cur;
                cur = cur->_left;
            }
            else if (cur->_kv.first < kv.first)
            {
                parent = cur;
                cur = cur->_right;
            }
            else
            {
                return make_pair(cur, false);
            }
        }

        //创建链接节点
        cur = new Node(kv);
        Node *newnode = cur;
        if (parent->_kv.first > kv.first)
        {
            parent->_left = cur;
        }
        else
        {
            parent->_right = cur;
        }
        cur->_parent = parent;

        //父节点存在且为红，则需要调整（不能存在连续的红色节点）
        while (parent && parent->_col == RED)
        {
            //此时当前节点一定有祖父节点
            Node *granparent = parent->_parent;
            //具体调整情况主要看叔叔节点
            //分左右讨论
            if (parent == granparent->_left)
            {
                Node *uncle = granparent->_right;
                //情况1：叔叔节点存在且为红
                if (uncle && uncle->_col == RED)
                {
                    //修改颜色，继续向上检查
                    granparent->_col = RED;
                    parent->_col = uncle->_col = BLACK;

                    cur = granparent;
                    parent = cur->_parent;
                }
                else //情况2和3：叔叔节点不存在 或者存在且为黑
                {
                    //单旋(三代节点为斜线)+变色
                    if (cur == parent->_left)
                    {
                        RotateR(granparent);

                        granparent->_col = RED;
                        parent->_col = BLACK;
                    }
                    else //双旋(三代节点为折线)+变色
                    {
                        RotateL(parent);
                        RotateR(granparent);

                        cur->_col = BLACK;
                        granparent->_col = RED;
                    }
                    //旋转后不需再向上调整了
                    break;
                }
            }
            else // parent=grandparent->right
            {
                Node *uncle = granparent->_left;
                if (uncle && uncle->_col == RED)
                {
                    parent->_col = uncle->_col = BLACK;
                    granparent->_col = RED;

                    cur = granparent;
                    parent = cur->_parent;
                }
                else
                {
                    if (cur == parent->_right)
                    {
                        RotateL(granparent);

                        parent->_col = BLACK;
                        granparent->_col = RED;
                    }
                    else
                    {
                        RotateR(parent);
                        RotateL(granparent);

                        cur->_col = BLACK;
                        granparent->_col = RED;
                    }
                    break;
                }
            }
        }

        //确保根节点为黑
        _root->_col = BLACK;
        return make_pair(newnode, true);
    }

    //判断是否满足红黑树
    bool IsRBTree()
    {
        if (_root == nullptr)
        {
            return true;
        }

        if (_root->_col == RED)
        {
            cout << "根节点为红色" << endl;
            return false;
        }

        int Blacknum = 0;
        Node *cur = _root;
        while (cur)
        {
            if (cur->_col == BLACK)
                Blacknum++;
            cur = cur->_left;
        }

        int i = 0;
        return _IsRBTree(_root, Blacknum, i);
    }

private:
    bool _IsRBTree(Node *root, int blacknum, int count)
    {
        if (root == nullptr)
        {
            if (blacknum == count)
                return true;
            cout << "各路径上黑色节点个数不同" << endl;
            return false;
        }

        if (root->_col == RED && root->_parent->_col == RED)
        {
            cout << "存在连续红色节点" << endl;
            return false;
        }

        if (root->_col == BLACK)
            count++;

        return _IsRBTree(root->_left, blacknum, count) && _IsRBTree(root->_right, blacknum, count);
    }

    void RotateL(Node *parent)
    {
        Node *subR = parent->_right;
        Node *subRL = subR->_left;
        Node *parentP = parent->_parent;

        parent->_right = subRL;
        if (subRL)
        {
            subRL->_parent = parent;
        }

        subR->_left = parent;
        parent->_parent = subR;

        if (parent == _root)
        {
            _root = subR;
            subR->_parent = nullptr;
        }
        else
        {
            subR->_parent = parentP;
            if (parentP->_left == parent)
            {
                parentP->_left = subR;
            }
            else
            {
                parentP->_right = subR;
            }
        }
    }

    void RotateR(Node *parent)
    {
        Node *subL = parent->_left;
        Node *subLR = subL->_right;
        Node *parentP = parent->_parent;

        parent->_left = subLR;
        if (subLR)
        {
            subLR->_parent = parent;
        }

        subL->_right = parent;
        parent->_parent = subL;

        if (parent == _root)
        {
            _root = subL;
            subL->_parent = nullptr;
        }
        else
        {
            subL->_parent = parentP;
            if (parentP->_left == parent)
            {
                parentP->_left = subL;
            }
            else
            {
                parentP->_right = subL;
            }
        }
    }

private:
    Node *_root;
};
/*
void TestRBTree()
{
    // int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
    // int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
    RBTree<int, int> t;
    int n = 1000000;
    srand(time(0));
    for (int i = 0; i < n; ++i)
    {
        int e = rand();
        t.Insert(make_pair(e, e));
    }

    // t.InOrder();
    cout << t.IsRBTree() << endl;
}
*/
void test_RBTree()
{
    int a[] = {4, 2, 6, 1, 3, 5, 15, 7, 16, 14};
    RBTree<int, int> t;
    for (auto e : a)
    {
        t.Insert(make_pair(e, e));
    }
    cout << t.IsRBTree() << endl;
}

void solve_RBTree(int n)
{
    int opt, x;
    RBTree<int, int> tree;
    while (n--)
    {
        cin >> opt >> x;

        if (opt == 1) // opt = 1：插入x
        {
            tree.Insert(make_pair(x, x));
        }
        else if (opt == 2) // opt = 2：删除x (若有多个相同的数，删除一个即可)
        {
        }
        else if (opt == 3)
        {
        }
        else if (opt == 4)
        {
        }
        else if (opt == 5)
        {
        }
        else if (opt == 6)
        {
        }
        else
            cout << "illegal input!" << endl;
    }
}

int main()
{
    int n;
    cin >> n;
    solve_RBTree(n);

    // test_RBTree();
    //  TestRBTree();
    return 0;
}

/*
输入格式
第一行为一个正整数n(n<=1e5)，表示操作的数目
接下来的n行，每行有2个整数opt、x，不同的opt(1<=opt<=6)取值代表的操作如下
opt = 1：插入x
opt = 2：删除x (若有多个相同的数，删除一个即可)
opt = 3：查询x的排名(定义为比x小的数的个数 +1)
opt = 4：查询排名为x的数
opt = 5：求x的前驱(定义为小于x的最大数)
opt = 6：求x的后继(定义为大于x的最小数)
输出格式
对于操作3-6，每行输出一个数表示答案
*/