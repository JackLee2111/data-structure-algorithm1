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
#include <iostream>
using namespace std;

int num; //中序遍历计数
int num_ans;
template <typename T>
struct AVLTreeNode
{
    T avl_key;      // 关键字
    int avl_height; // 高度
    int size;
    AVLTreeNode *avl_leftChild;  // 左孩子
    AVLTreeNode *avl_rightChild; // 右孩子
    AVLTreeNode *parent;         //父节点
    AVLTreeNode(T value, AVLTreeNode *l, AVLTreeNode *r) : avl_key(value), avl_height(0), avl_leftChild(l), avl_rightChild(r) {}
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

    //以下为函数的具体实现
    //中序遍历
    void inOrder(AVLTreeNode<T> *node)
    {
        if (node == NULL)
            return;
        inOrder(node->avl_leftChild);
        cout << node->avl_key << " ";
        inOrder(node->avl_rightChild);
    }

    //查询排名为x的数，中序遍历————————————
    T getTthNum(int x)
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

    //查询数x的排名，中序遍历——————————————
    void find_rank(T x)
    {
        num = 0;
        num_ans = 0;
        inOrder3(avl_root, x);
        cout << num_ans << endl;
    }
    void inOrder3(AVLTreeNode<T> *node, int x)
    {
        if (node == NULL)
            return;
        inOrder3(node->avl_leftChild, x);
        num++;
        if (node->avl_key == x)
            num_ans = num;
        inOrder3(node->avl_rightChild, x);
    }

    //值为key的直接后继 TODO
    /*
    AVLTreeNode<T> *find_next_node(T x)
    {
        AVLTreeNode<T> *parent = nullptr;
        AVLTreeNode<T> *current = avl_root;
        AVLTreeNode<T> *t = nullptr;
        while (current) //当当前节点不为空时
        {
            if (current->avl_key > x)
            {
                parent = current;
                current = current->avl_leftChild;
            }
            else if (current->avl_key < x)
            {
                parent = current;
                current = current->avl_rightChild;
            }
            else
            {
                t = Rt_node(current);
                break;
            }
        }
        if (current->avl_key == key)
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
    AVLTreeNode<T> *rc_node(AVLTreeNode<T> *x) //后继
    {
        Node *tmp = x;
        if (tmp->GetRt() == e_node)
        {
            while (tmp->Getpre() != e_node && tmp->Getpre()->GetRt() == tmp)
            {
                tmp = tmp->Getpre();
            }
            tmp = tmp->Getpre();
        }
        else
        {
            tmp = tmp->GetRt();
            while (tmp->GetLt() != e_node)
            {
                tmp = tmp->GetLt();
            }
        }
        return tmp;
    }
    */

    //高度
    int height(AVLTreeNode<T> *node)
    {
        return node != NULL ? node->avl_height : 0;
    }

    //销毁二叉树
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
    //寻找二叉树中的值为key的节点，返回该点的指针
    AVLTreeNode<T> *search(AVLTreeNode<T> *node, T key)
    {
        if (node == NULL || node->avl_key == key)
            return node;
        if (key < node->avl_key)
            return search(node->avl_leftChild, key);
        else
            return search(node->avl_rightChild, key);
    }

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
            node = new AVLTreeNode<T>(key, NULL, NULL);
        }
        else if (key < node->avl_key) // key插入node的左子树的情况
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
        else
        {
            cout << "添加失败，不能添加相同的结点" << endl;
        }
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
        else if (z->avl_key > node->avl_key) //待删除的节点在node的右子树中
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
        else //当前node就是要删除的节点
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
     *       k2              k1
     *      /  \            /  \
     *     k1   z   ===>   x    k2
     *    /  \                 /  \
     *   x    y               y    z
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
     *       k1              k2
     *      /  \            /  \
     *     x   k2   ===>   k1   z
     *        /  \        /  \
     *       y    z      x    y
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
     *       k3               k3               k2
     *      /  \     RR      /  \     LL      /  \
     *     k1   D   ===>   k2    D   ===>   k1    k3
     *    /  \            /  \             /  \  /  \
     *   A    K2         k1   C           A    B C   D
     *       /  \       /  \
     *      B    C     A    B
     */
    AVLTreeNode<T> *leftRightRotation(AVLTreeNode<T> *&k3)
    {
        k3->avl_leftChild = rightRightRotation(k3->avl_leftChild);
        return leftLeftRotation(k3);
    }

    /* RL：右子树的左边失去平衡(右双旋转)
     *     k1               k1                K2
     *    /  \      LL     /  \      RR      /  \
     *   A    k3   ===>   A    k2   ===>   k1    K3
     *       /  \             /  \        /  \  /  \
     *      k2   D           B    k3     A    B C   D
     *     /  \                  /   \
     *    B    D                C     D
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
        if (opt == 1) //插入x
        {
            tree.insert(x);
        }
        else if (opt == 2) //删除x (若有多个相同的数，删除一个即可)
        {
            tree.remove(x);
        }

        else if (opt == 3) //查询x的排名(定义为比x小的数的个数 +1)
        {
            tree.find_rank(x);
        }
        else if (opt == 4) // opt = 4：查询排名为x的数
        {
            tree.getTthNum(x);
        }
        /*
        else if (opt == 5)
        {
            cout << mp->Lfind(x)->Getdata() << endl;
        }
        else if (opt == 6)
        {
            cout << mp->Rfind(x)->Getdata() << endl;
        }*/
    }
    /*
    tree.insert(3);
    tree.insert(2);
    tree.insert(1);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);

    tree.insert(16);
    tree.insert(15);
    tree.insert(14);
    tree.insert(13);
    tree.insert(12);
    tree.insert(11);
    tree.insert(10);
    tree.insert(8);
    tree.insert(9);
    tree.remove(8);
    tree.inOrder();
    cout << endl;
    */
}

int main()
{
    int n;
    cin >> n;
    solve_AVLTree(n);

    // tree.preOrder();
    // cout << endl;

    // tree.remove(8);
    // tree.preOrder();
    // cout << endl;
    // tree.preOrder();
    // cout << endl;

    return 0;
}
