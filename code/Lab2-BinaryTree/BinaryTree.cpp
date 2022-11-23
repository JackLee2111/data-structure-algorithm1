#include <iostream>
#include <stack>
#include <queue>
using namespace std;

// BinNode状态与性质的判断
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))     //至少拥有一个孩子
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //同时拥有两个孩子
#define IsLeaf(x) (!HasChild(x))

//与BinNode具有特定关系的节点及指针
#define sibling(p) /*兄弟*/ \
    (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc)

#define uncle(x) /*叔叔*/ \
    (IsLChild(*((x)->parent)) ? (x)->parent->parent->rc : (x)->parent->parent->lc)

#define FromParentTo(x) /*来自父亲的引用*/ \
    (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lc : (x).parent->rc))

#define BinNodePosi(T) BinNode<T> *         //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度（与“空树高度为-1”的约定相统一）

//节点颜色
typedef enum
{
    RB_RED,
    RB_BLACK
} RBColor;
template <typename T>
struct BinNode //二叉树节点模板类
{
    // 成员（为简化描述起见统一开放，读者可根据需要进一步封装）
    T data; //数值
    BinNodePosi(T) parent;
    BinNodePosi(T) lc;
    BinNodePosi(T) rc; //父节点及左、右孩子
    int height;        //高度（通用）
    int npl;           // Null Path Length（左式堆，也可直接用height代替）
    RBColor color;     //颜色（红黑树）
    // 构造函数
    BinNode() : parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) {}
    BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
            int h = 0, int l = 1, RBColor c = RB_RED) : data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) {}
    // 操作接口
    int size();                           //统计当前节点后代总数，亦即以其为根的子树的规模
    BinNodePosi(T) insertAsLC(T const &); //作为当前节点的左孩子插入新节点
    BinNodePosi(T) insertAsRC(T const &); //作为当前节点的右孩子插入新节点
    BinNodePosi(T) succ();                //取当前节点的直接后继
    template <typename VST>
    void travLevel(VST &); //子树层次遍历
    template <typename VST>
    void travPre(VST &); //先序遍历
    template <typename VST>
    void travIn(VST &); //中序遍历
    template <typename VST>
    void travPost(VST &); //后序遍历
    // 比较器、判等器
    bool operator<(BinNode const &bn) { return data < bn.data; }   //小于
    bool operator==(BinNode const &bn) { return data == bn.data; } //等于
};
//操作接口
//将e作为当前节点的左孩子插入二叉树
template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const &e)
{
    return lc = new BinNode(e, this);
}

//将e作为当前节点的右孩子插入二叉树
template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const &e)
{
    return rc = new BinNode(e, this);
}

//二叉树中序遍历算法统一入口
/*
template <typename T>
template <typename VST> //元素类型、操作器
void BinNode<T>::travIn(VST &visit)
{
    switch (rand() % 5) //此处暂随机选择以做测试，共五种选择
    {
    case 1:
        travIn_I1(this, visit);
        break; //迭代版#1
    case 2:
        travIn_I2(this, visit);
        break; //迭代版#2
    case 3:
        travIn_I3(this, visit);
        break; //迭代版#3
    case 4:
        travIn_I4(this, visit);
        break; //迭代版#4
    default:
        travIn_R(this, visit);
        break; //递归版
    }
}
*/

//二叉树规模计数
template <typename T>
int BinNode<T>::size()
{
    int sum = 1;
    if (lc)
    {
        sum += lc->size();
    }
    if (rc)
    {
        sum += rc->size();
    }
    return sum;
}

//二叉树模板类
template <typename T>
class BinTree
{
protected:
    int _size;
    BinNodePosi(T) _root;                       //规模、根节点
    virtual int updateHeight(BinNodePosi(T) x); //更新节点x的高度
    void updateHeightAbove(BinNodePosi(T) x);   //更新节点x及其祖先的高度
public:
    BinTree() : _size(0), _root(NULL) {} //构造函数
    ~BinTree()                           //析构函数
    {
        if (0 < _size)
            remove(_root);
    }
    int size() const { return _size; }                           //规模
    bool empty() const { return !_root; }                        //判空
    BinNodePosi(T) root() const { return _root; }                //树根
    BinNodePosi(T) insertAsRoot(T const &e);                     //插入根节点
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const &e);     // e作为x的左孩子（原无）插入
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const &e);     // e作为x的右孩子（原无）插入
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T> *&S); // T作为x左子树接入
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T> *&S); // T作为x右子树接入
    int remove(BinNodePosi(T) x);                                //删除以位置x处节点为根的子树，返回该子树原先的规模
    BinTree<T> *secede(BinNodePosi(T) x);                        //将子树x从当前树中摘除，并将其转换为一棵独立子树

    //层次遍历
    template <typename VST>
    void travLevel(VST &visit)
    {
        if (_root)
            _root->travLevel(visit);
    }
    //先序遍历
    template <typename VST> //操作器
    void travPre(VST &visit)
    {
        if (_root)
            _root->travPre(visit);
    }
    //中序遍历
    template <typename VST> //操作器
    void travIn(VST &visit)
    {
        if (_root)
            _root->travIn(visit);
    }
    //后序遍历
    template <typename VST> //操作器
    void travPost(VST &visit)
    {
        if (_root)
            _root->travPost(visit);
    }
    bool operator<(BinTree<T> const &t) //比较器
    {
        return _root && t._root && lt(_root, t._root);
    }
    bool operator==(BinTree<T> const &t) //判等器
    {
        return _root && t._root && (_root == t._root);
    }
};

//高度更新
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) //更新节点x高度
{
    return x->height = 1 + max(stature(x->lc), stature(x->rc));
} //具体规则，因树而异
template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) //更新高度
{
    while (x)
    {
        updateHeight(x);
        x = x->parent;
    }
} //从x出发，覆盖历代祖先。可优化

//结点插入
//将e当作根节点插入空的二叉树
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const &e)
{
    _size = 1;
    return _root = new BinNode<T>(e);
}

// e插入为x的左孩子
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const &e)
{
    _size++;
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x->lc;
}

// e插入为x的右孩子
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const &e)
{
    _size++;
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x->rc;
}

//子树接入
//二叉树子树接入算法：将S当作节点x的左子树接入，S本身置空
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T> *&S) // x->lc == NULL
{
    if (x->lc = S->_root)
        x->lc->parent = x; //接入
    _size += S->_size;
    updateHeightAbove(x); //更新全树规模与x所有祖先的高度
    S->_root = NULL;
    S->_size = 0;
    release(S);
    S = NULL;
    return x; //释放原树，返回接入位置
}

//二叉树子树接入算法：将S当作节点x的右子树接入，S本身置空
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T> *&S) // x->rc == NULL
{
    if (x->rc = S->_root)
        x->rc->parent = x; //接入
    _size += S->_size;
    updateHeightAbove(x); //更新全树规模与x所有祖先的高度
    S->_root = NULL;
    S->_size = 0;
    release(S);
    S = NULL;
    return x; //释放原树，返回接入位置
}

//子树删除
//删除二叉树中位置x处的节点及其后代，返回被删除节点的数值

template <typename T>
int BinTree<T>::remove(BinNodePosi(T) x) // assert: x为二叉树中的合法位置
{
    FromParentTo(*x) = NULL;      //切断来自父节点的指针
    updateHeightAbove(x->parent); //更新祖先高度
    int n = removeAt(x);
    _size -= n;
    return n; //删除子树x，更新规模，返回删除节点总数
}
//删除二叉树中位置x处的节点及其后代，返回被删除节点的数值

template <typename T>
static int removeAt(BinNodePosi(T) x) // assert: x为二叉树中的合法位置
{
    if (!x)
        return 0;                                  //递归基：空树
    int n = 1 + removeAt(x->lc) + removeAt(x->rc); //递归释放左、右子树
    // release(x->data);
    free(x);
    return n; //释放被摘除节点，并返回删除节点总数
}

//子树分离
template <typename T>                            //二叉树子树分离算法：将子树x从当前树中摘除，将其封装为一棵独立子树返回
BinTree<T> *BinTree<T>::secede(BinNodePosi(T) x) // assert: x为二叉树中的合法位置
{
    FromParentTo(*x) = NULL;      //切断来自父节点的指针
    updateHeightAbove(x->parent); //更新原树中所有祖先的高度
    BinTree<T> *S = new BinTree<T>;
    S->_root = x;
    x->parent = NULL; //新树以x为根
    S->_size = x->size();
    _size -= S->_size;
    return S; //更新规模，返回分离出来的子树
}

//遍历
//先序遍历（递归版）
template <typename T, typename VST> //元素类型、操作器
void travPre_R(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    visit(x->data);
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
}

//后序遍历（递归版）
template <typename T, typename VST> //元素类型、操作器
void travPost_R(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    travPost_R(x->lc, visit);
    travPost_R(x->rc, visit);
    visit(x->data);
}

//中序遍历（递归版）
template <typename T, typename VST> //元素类型、操作器
void travIn_R(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    travIn_R(x->lc, visit);
    visit(x->data);
    travIn_R(x->rc, visit);
}

//二叉树先序遍历算法（迭代版#2）借助辅助栈
//从当前节点出发，沿左分支不断深入，直至没有左分支的节点；沿途节点遇到后立即访问
template <typename T, typename VST> //元素类型、操作器
static void visitAlongLeftBranch(BinNodePosi(T) x, VST &visit, stack<BinNodePosi(T)> &S)
{
    while (x)
    {
        visit(x->data); //访问当前节点
        S.push(x->rc);  //右孩子入栈暂存（可优化：通过判断，避免空的右孩子入栈）
        x = x->lc;      //沿左分支深入一层
    }
}

template <typename T, typename VST> //元素类型、操作器
void travPre_I2(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> S; //辅助栈
    while (true)
    {
        visitAlongLeftBranch(x, visit, S); //从当前节点出发，逐批访问
        if (S.empty())
            break;   //直到栈空退出
        x = S.top(); //弹出下一批的起点
        S.pop();
    }
}

//二叉树中序遍历算法（迭代版#1）
template <typename T> //从当前节点出发，沿左分支不断深入，直至没有左分支的节点
static void goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)> &S)
{
    while (x)
    {
        S.push(x);
        x = x->lc;
    } //当前节点入栈后随即向左侧分支深入，迭代直到无左孩子
}
template <typename T, typename VST> //元素类型、操作器
void travIn_I1(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> S; //辅助栈
    while (true)
    {
        goAlongLeftBranch(x, S); //从当前节点出发，逐批入栈
        if (S.empty())
            break; //直至所有节点处理完毕
        x = S.top();
        S.pop();
        visit(x->data); //弹出栈顶节点并访问之
        x = x->rc;      //转向右子树
    }
}
//中序遍历意义下的直接后继
template <typename T>
BinNodePosi(T) BinNode<T>::succ() //定位节点v的直接后继
{
    BinNodePosi(T) s = this; //记录后继的临时变量
    if (rc)                  //若有右孩子，则直接后继必在右子树中，具体地就是
    {
        s = rc; //右子树中
        while (HasLChild(*s))
            s = s->lc; //最靠左（最小）的节点
    }
    else //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
    {
        while (IsRChild(*s))
            s = s->parent; //逆向地沿右向分支，不断朝左上方移动
        s = s->parent;     //最后再朝右上方移动一步，即抵达直接后继（如果存在）
    }
    return s;
}

//二叉树中序遍历算法（迭代版#3，无需辅助栈）
template <typename T, typename VST> //元素类型、操作器
void travIn_I3(BinNodePosi(T) x, VST &visit)
{
    bool backtrack = false; //前一步是否刚从右子树回溯——省去栈，仅O(1)辅助空间
    while (true)
        if (!backtrack && HasLChild(*x)) //若有左子树且不是刚刚回溯，则
            x = x->lc;                   //深入遍历左子树
        else                             //否则——无左子树或刚刚回溯（相当于无左子树）
        {
            visit(x->data);    //访问该节点
            if (HasRChild(*x)) //若其右子树非空，则
            {
                x = x->rc;         //深入右子树继续遍历
                backtrack = false; //并关闭回溯标志
            }
            else //若右子树空，则
            {
                if (!(x = x->succ()))
                    break;        //回溯（含抵达末节点时的退出返回）
                backtrack = true; //并设置回溯标志
            }
        }
}

//二叉树的后序遍历（迭代版）
template <typename T>                           //在以S栈顶节点为根的子树中，找到最高左侧可见叶节点
static void gotoHLVFL(stack<BinNodePosi(T)> &S) //沿途所遇节点依次入栈
{
    while (BinNodePosi(T) x = S.top()) //自顶而下，反复检查当前节点（即栈顶）
        if (HasLChild(*x))             //尽可能向左
        {
            if (HasRChild(*x))
                S.push(x->rc); //若有右孩子，优先入栈
            S.push(x->lc);     //然后才转至左孩子
        }
        else               //实不得已
            S.push(x->rc); //才向右
    S.pop();               //返回之前，弹出栈顶的空节点
}
template <typename T, typename VST>
void travPost_I(BinNodePosi(T) x, VST &visit)
{
    stack<BinNodePosi(T)> S; //辅助栈
    if (x)
        S.push(x); //根节点入栈
    while (!S.empty())
    {
        if (S.top() != x->parent) //若栈顶非当前节点之父（则必为其右兄），此时需
            gotoHLVFL(S);         //在以其右兄为根之子树中，找到HLVFL（相当于递归深入其中）
        x = S.top();
        S.pop();
        visit(x->data); //弹出栈顶（即前一节点之后继），并访问之
    }
}

//二叉树层次遍历算法
//#include "../queue/queue.h" //引入队列
template <typename T>
template <typename VST> //元素类型、操作器
void BinNode<T>::travLevel(VST &visit)
{
    queue<BinNodePosi(T)> Q; //辅助队列
    Q.push(this);            //根节点入队
    while (!Q.empty())       //在队列再次变空之前，反复迭代
    {
        BinNodePosi(T) x = Q.front();
        Q.pop();
        visit(x->data); //取出队首节点并访问之
        if (HasLChild(*x))
            Q.push(x->lc); //左孩子入队
        if (HasRChild(*x))
            Q.push(x->rc); //右孩子入队
    }
}
void print(char c)
{
    cout << c;
}

void testPrint(BinTree<char> &tree)
{
    cout << "递归遍历" << endl;
    //从根节点开始先序遍历（递归）
    cout << "先序遍历（递归）：";
    travPre_R(tree.root(), print);
    cout << endl;

    //从根节点开始中序遍历（递归）
    cout << "中序遍历（递归）: ";
    travIn_R(tree.root(), print);
    cout << endl;

    //从根节点开始后序遍历（递归）
    cout << "后序遍历（递归）: ";
    travPost_R(tree.root(), print);
    cout << endl;

    cout << endl;
    cout << "非递归遍历" << endl;

    //先序遍历（迭代）
    cout << "先序遍历（迭代）：";
    travPre_I2(tree.root(), print);
    cout << endl;

    //中序遍历（迭代1）
    cout << "中序遍历（迭代1）：";
    travIn_I1(tree.root(), print);
    cout << endl;

    //中序遍历（迭代2）
    cout << "中序遍历（迭代2）：";
    travIn_I3(tree.root(), print);
    cout << endl;

    //后序遍历（迭代）
    cout << "后序遍历（迭代）：";
    travPost_I(tree.root(), print);
    cout << endl;

    cout << endl;

    //从根节点开始层次遍历
    cout << "层次遍历: ";
    tree.root()->travLevel(print);
    cout << endl;
}

//中序遍历构建一棵二叉树
/*
void CreateBinTree_in(BinNodePosi(char) & x)
{
    char ch;
    cin >> ch;
    if (ch == '#')
    {
        x = NULL;
    }
    else
    {
        x = new BinNode(char);
        //判断空间是否创建成功
        if (x == NULL)
            exit(0);
        //递归左子树
        CreateBinTree_in(x->lc);
        //生成根结点
        x->data = ch;
        //递归右子树
        CreateBinTree_in(x->rc);
    }
}
*/

//叶子节点个数
int count_leaf_number(BinNode<char> *node)
{
    int sum = 0;
    if (IsLeaf(*node))
        sum++;
    if (node->lc)
    {
        sum += count_leaf_number(node->lc);
    }
    if (node->rc)
    {
        sum += count_leaf_number(node->rc);
    }
    return sum;
}

//该节点的兄弟
void get_sibling(BinNode<char> *node)
{
    //根节点无兄弟
    if (IsRoot(*node))
    {
        cout << "节点" << node->data << "为根节点，没有兄弟!" << endl;
        return;
    }

    if (sibling(node) != NULL)
        cout << "节点" << node->data << "的兄弟为：" << sibling(node)->data << endl;
}
//该节点的叔叔
void get_uncle(BinNode<char> *node)
{
    if (IsRoot(*node))
    {
        cout << "节点" << node->data << "为根节点，没有叔叔!" << endl;
        return;
    }
    if (IsRoot(*(node->parent)))
    {
        cout << "节点" << node->data << "的父母为根节点，该节点没有叔叔!" << endl;
        return;
    }
    if (uncle(node) != NULL)
        cout << "节点" << node->data << "的叔叔为：" << uncle(node)->data << endl;
}
//该节点的堂兄弟
void get_cousin(BinNode<char> *node)
{
    if (IsRoot(*node))
    {
        cout << "节点" << node->data << "为根节点，没有堂兄弟!" << endl;
        return;
    }
    if (IsRoot(*(node->parent)))
    {
        cout << "节点" << node->data << "的父母为根节点，该节点没有堂兄弟!" << endl;
        return;
    }
    if (uncle(node) != NULL)
    {
        if (HasChild(*uncle(node)))
        {
            char c1 = uncle(node)->lc->data;
            char c2 = uncle(node)->rc->data;
            cout << "节点" << node->data << "的堂兄弟为：";
            if (c1)
                cout << c1 << " ";
            if (c2)
                cout << c2 << " ";
            cout << endl;
        }
        else
            cout << "节点" << node->data << "没有堂兄弟!" << endl;
    }
}
int main()
{
    BinTree<char> tree;

    tree.insertAsRoot('R');

    tree.root()->insertAsLC('A');
    tree.root()->insertAsRC('B');

    tree.root()->lc->insertAsLC('C');
    tree.root()->lc->insertAsRC('D');
    tree.root()->rc->insertAsLC('E');
    tree.root()->rc->insertAsRC('F');

    tree.root()->lc->lc->insertAsLC('G');
    tree.root()->lc->lc->insertAsRC('H');
    tree.root()->lc->rc->insertAsLC('I');
    tree.root()->rc->lc->insertAsLC('J');
    tree.root()->rc->lc->insertAsRC('L');

    testPrint(tree);
    cout << endl;

    //二叉树规模
    cout << "二叉树规模" << tree.root()->size() << endl;

    //叶节点个数
    cout << "叶节点个数" << count_leaf_number(tree.root()) << endl;

    //兄弟节点
    get_sibling(tree.root());
    get_sibling(tree.root()->lc);
    get_sibling(tree.root()->lc->lc);
    cout << endl;

    //叔叔节点
    get_uncle(tree.root());
    get_uncle(tree.root()->lc);
    get_uncle(tree.root()->lc->lc);
    cout << endl;

    //堂兄弟节点
    get_cousin(tree.root());
    get_cousin(tree.root()->lc);
    get_cousin(tree.root()->lc->lc);

    return 0;
}
