#include <iostream>

using namespace std;

template <class T>
struct BinTreeNode
{
    T data;
    BinTreeNode<T> *leftchild;
    BinTreeNode<T> *rightchild;
    BinTreeNode()
    {
        leftchild = NULL;
        rightchild = NULL;
    }
    BinTreeNode(T x)
    {
        data = x;
        leftchild = NULL;
        rightchild = NULL;
    }
};

template <class T>
class Tree
{
public:
    BinTreeNode<T> *root;
    Tree();
    ~Tree();
    void CreateTree(BinTreeNode<T> *&t);
    void deleTree(BinTreeNode<T> *t);
    BinTreeNode<T> *Parent(BinTreeNode<T> *root, T item);     //返回item的父结点指针
    BinTreeNode<T> *LeftChild(BinTreeNode<T> *root, T item);  //返回item的左孩子指针
    BinTreeNode<T> *RightChild(BinTreeNode<T> *root, T item); //返回item的右孩子指针
};

template <class T>
Tree<T>::Tree()
{
    root = NULL;
}

template <class T>
Tree<T>::~Tree()
{
}

template <class T>
void Tree<T>::CreateTree(BinTreeNode<T> *&t)
{
    T x;
    cin >> x;
    if (x == '#')
    {
        t = NULL;
        return;
    }
    t = new BinTreeNode<T>(x);
    if (t == NULL)
        return;
    CreateTree(t->leftchild);
    CreateTree(t->rightchild);
}

template <class T>
void Tree<T>::deleTree(BinTreeNode<T> *t)
{
    if (t == NULL)
        return;
    deleTree(t->leftchild);
    deleTree(t->rightchild);
    delete t;
}

template <class T>
BinTreeNode<T> *Tree<T>::Parent(BinTreeNode<T> *root, T item)
{
    BinTreeNode<T> *temp = NULL;
    if (root == NULL)
        return NULL;
    if (root->leftchild != NULL)
    {
        if (root->leftchild->data == item)
            return root;
    }
    if (root->rightchild != NULL)
    {
        if (root->rightchild->data == item)
            return root;
    }
    temp = Parent(root->leftchild, item);
    if (temp)
        return temp;
    temp = Parent(root->rightchild, item);
    if (temp)
        return temp;
    return NULL;
}

template <class T>
BinTreeNode<T> *Tree<T>::LeftChild(BinTreeNode<T> *root, T item)
{
    BinTreeNode<T> *temp;
    if (root == NULL)
        return NULL;
    if (root->data == item)
        return root->leftchild;
    temp = LeftChild(root->leftchild, item);
    if (temp)
        return temp;
    temp = LeftChild(root->rightchild, item);
    if (temp)
        return temp;
}

template <class T>
BinTreeNode<T> *Tree<T>::RightChild(BinTreeNode<T> *root, T item)
{
    BinTreeNode<T> *temp;
    if (root == NULL)
        return NULL;
    if (root->data == item)
        return root->rightchild;
    temp = RightChild(root->leftchild, item);
    if (temp)
        return temp;
    temp = RightChild(root->rightchild, item);
    if (temp)
        return temp;
}

int main()
{
    Tree<char> temp;
    temp.CreateTree(temp.root);
    char ch;
    cin >> ch;
    BinTreeNode<char> *a, *b, *c;
    a = temp.Parent(temp.root, ch);
    if (a)
        cout << a->data << endl;
    else
        cout << "NULL" << endl;
    b = temp.LeftChild(temp.root, ch);
    if (b)
        cout << b->data << endl;
    else
        cout << "NULL" << endl;
    c = temp.RightChild(temp.root, ch);
    if (c)
        cout << c->data << endl;
    else
        cout << "NULL" << endl;
    temp.deleTree(temp.root);
    return 0;
}
