template <typename T>
using BinNodePosi = BinNode<T> *; //节点位置

// BinNode 模板类
template <typename T>
struct BinNode
{
    //成员
    T data;
    BinNodePosi(T) parent; //父亲
    BinNodePosi(T) lc;     //左孩子
    BinNodePosi(T) rc;     //右孩子
    int height;

    //构造函数
    BinNode() : parent(NULL), lc(NULL), rc(NULL), height(0) {}
    BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, int h = 0) : data(e), parent(p), lc(lc), rc(rc), hright(h) {}

    //操作接口
    int size();                           //当前节点后代总数（子树规模）
    BinNodePosi(T) insertAsLc(T const &); //作为左子树插入
    BinNodePosi(T) insertAsRc(T const &); //作为右子树插入
    BinNodePosi(T) succ;                  //（中序遍历意义下）当前节点的直接后继
    template <typename VST>
    void travLevel(VST &); //层次遍历
    template <typename VST>
    void travPre(VST &); //先序遍历
    template <typename VST>
    void travIn(VST &); //中序遍历
    template <typename VST>
    void travPOost(VST &); //后序遍历

    //比较器、判等器
    bool operator<(BinNode const &bn)
    {
        return data < bn.data;
    }
    bool operator==(BinNode const &bn)
    {
        return data == bn.data;
    }
};

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLc(T const &e)
{
    return lc = new BinNode(e, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRc(T const &e)
{
    return rc = new BinNode(e, this);
}

template <typename T>
int BinNode<T>::size()//后代总数，即以其为根的子树的规模
{
    int s = 1;//计算本身
    if (lc)
        s += lc->size();//递归计入左子树规模
    if (rc)
        s += rc->size();//递归计入右子树规模
    return s;
}