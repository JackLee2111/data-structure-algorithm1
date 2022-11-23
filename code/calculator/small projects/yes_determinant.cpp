//行列式求值
//已经植入vector操作

#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cstdlib>
using namespace std;

//以下为Vector构造
typedef int Rank;          //定义秩
#define DEFAULT_CAPACITY 3 //默认初始容量

template <typename T>
class Vector
{
protected:
    //基本成员
    Rank _size;    //元素个数
    int _capacity; //实际空间
    T *_elem;      //元素指针

    //其他内部函数
    void copyFrom(T const *A, Rank lo, Rank hi); //从A中复制区间[lo, hi)
    void expand();                               //空间不足时扩容
    void shrink();                               //装填因子过小时压缩空间

public:
    //构造函数
    Vector(int c = DEFAULT_CAPACITY, int s = 0)
    { //默认构造
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s;)
            ;
    }
    //复制构造接口
    Vector(T const *A, Rank n) { copyFrom(A, 0, n); }                           //从数组复制
    Vector(T const *A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }               //复制数组区间
    Vector(Vector<T> const &V) { copyFrom(V._elem, 0, V._size); }               //拷贝构造
    Vector(Vector<T> const &V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //复制向量区间

    //析构函数
    ~Vector() { delete[] _elem; } //删除数组

    //其他接口函数

    //只读接口
    T get(Rank r);                                                                    //获取秩为r的元素值
    int capacity() const { return _capacity; }                                        //获取容量
    Rank size() const { return _size; }                                               //返回最大秩
    bool empty() const { return !_size; }                                             //判空
    Rank find(T const &e, Rank lo, Rank hi) const;                                    //无序向量区间查找
    Rank find(T const &e) const { return find(e, 0, _size); }                         //无序向量整体查找
    Rank search(T const &e, Rank lo, Rank hi) const;                                  //有序向量区间查找
    Rank search(T const &e) const { return (_size <= 0) ? -1 : search(e, 0, _size); } //有序向量整体查找

    //可写接口
    T &operator[](Rank r) const;                         //重载[]操作符，使其能够像数组一样引用元素
    Vector<T> &operator=(Vector<T> const &);             //重载=操作符，使其能够向数组一样赋值
    void put(Rank r, T const &e);                        //向量修改
    int remove(Rank lo, Rank hi);                        //删除区间[lo,hi)的元素，并返回删除的元素个数
    T remove(Rank r);                                    //删除秩为r的元素，并返回被删除的元素值void unsort() { unsort(0, _size); }                  //向量置乱
    void unsort(Rank lo, Rank hi);                       //对[lo, hi)区间置乱
    void reverse() { reverse(0, _size); }                //向量逆序
    void reverse(Rank lo, Rank hi);                      //对[lo, hi]区间逆序
    Rank insert(Rank r, T const &e);                     //在秩为r的位置插入元素e
    Rank insert(T const &e) { return insert(_size, e); } //默认在末尾插入元素e

    int deduplicate(); //无序去重
    int uniquify();    //有序去重

    //遍历操作
    void traverse(void (*)(T &)); //使用函数指针操作
    template <typename VST>
    void traverse(VST &); //使用函数对象操作

    //排序
    bool bubble(Rank lo, Rank hi);         //冒泡扫描交换
    void bubbleSort(Rank lo, Rank hi);     //冒泡排序
    void mergeSort(Rank lo, Rank hi);      //归并排序
    void merge(Rank lo, Rank mi, Rank hi); //二路归并

}; // Vector

// copyFrom()方法
template <typename T>
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{
    _elem = new T[_capacity = 2 * (hi - lo)]; //申请空间
    _size = 0;                                //规模置零
    while (lo < hi)
    {
        _elem[_size++] = A[lo++]; //逐个复制
    }
}
//重载 =
template <typename T>
Vector<T> &Vector<T>::operator=(const Vector<T> &V)
{
    delete[] _elem; //删除原有空间，因为下面会申请新的空间
    copyFrom(V._elem, 0, V._size);
    return *this; //返回值为引用便于链式赋值
}

//重载 []
template <typename T>
T &Vector<T>::operator[](Rank r) const
{
    return _elem[r]; //返回值为引用，这样就可以实现链式赋值（即连等）
}

//加倍扩容expend()
template <typename T>
void Vector<T>::expand()
{
    if (_size < _capacity)
        return;
    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY;

    T *oldElem = _elem;
    _elem = new T[_capacity <<= 1]; //申请两倍的新的空间
    for (int i = 0; i < _size; i++)
    {
        _elem[i] = oldElem[i]; //若T为非基本类型，则该类型需重载=操作符
    }
    delete[] oldElem; //释放原空间
}

//缩容shrink()
template <typename T>
void Vector<T>::shrink()
{
    while (_size << 2 < _capacity)
    { //若实际规模不到容量的1/4，则缩容
        T *oldElem = _elem;
        _elem = new T[_capacity >>= 1]; //申请原来一半的空间
        for (int i = 0; i < _size; i++)
        {
            _elem[i] = oldElem[i]; //若T为非基本类型，则该类型需重载=操作符
        }
        delete[] oldElem; //释放原空间
    }
}

//向量置乱
template <typename T>
void permute(Vector<T> &V)
{
    for (int i = V.size(); i > 0; i--)
    {
        swap(V[i - 1], V[rand() % i]);
    }
}

//封装置乱
template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi)
{
    T *V = _elem + lo; //调整指针
    for (Rank i = hi - lo; i > 0; i--)
    {
        std::swap(V[i - 1], V[rand() % i]);
    }
}

//封装逆序
template <typename T>
void Vector<T>::reverse(Rank lo, Rank hi)
{
    T *V = _elem; //调整指针
    Rank l = lo, r = hi - 1;
    while (l < r)
    {
        std::swap(V[l], V[r]);
        l++, r--;
    }
}

//顺序查找
template <typename T>
Rank Vector<T>::find(T const &e, Rank lo, Rank hi) const
{
    while ((lo < hi--) && (e != _elem[hi]))
        ;      //当匹配到对应的e后停止，并返回秩
    return hi; //若查找失败，会返回lo - 1
}

// 二分查找
template <typename T>
Rank Vector<T>::search(T const &e, Rank lo, Rank hi) const
{ //在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
    T *A = _elem;
    while (lo < hi)
    {                                        //每步迭代仅需做一次比较判断，有两个分支
        Rank mi = (lo + hi) >> 1;            //以中点为轴点
        (e < A[mi]) ? hi = mi : lo = mi + 1; //经比较后确定深入[lo, mi)或(mi, hi)
    }                                        //成功查找不能提前终止
    Rank p = --lo;                           //循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
    if (A[p] == e)
        return p; //有多个命中元素时，总能保证返回秩最大者
    return -1;    //查找失败时，返回 -1
}

//获取秩为r的元素值
template <typename T>
T Vector<T>::get(Rank r)
{
    T value = _elem[r]; //备份将被删除的元素
    return value;       //反回被删除的元素
}

//修改
template <typename T>
void Vector<T>::put(Rank r, T const &e)
{
    _elem[r] = e;
}

//插入
template <typename T>
Rank Vector<T>::insert(Rank r, T const &e)
{             //将e作为秩为r元素插入
    expand(); //若需要，先扩容
    for (int i = _size; i > r; i--)
        _elem[i] = _elem[i - 1]; //整体后移一位，从后向前
    _elem[r] = e, _size++;       //置入e并更新容量
    return r;                    //返回秩
}

//区间删除
template <typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{
    if (lo == hi)
        return 0; //出于效率考虑，单独处理退化情况，如remove(0,0)
    while (hi < _size)
    {
        _elem[lo++] = _elem[hi++]; //整体前移，若删除区间大于其后缀区间，未覆盖部分不做处理，下次缩容时自动消除
    }
    _size = lo;     //确定新界限
    shrink();       //若装填因子过小，缩容
    return hi - lo; //返回删除元素的个数
}

//删除秩为r的元素
template <typename T>
T Vector<T>::remove(Rank r)
{
    T re_elem = _elem[r]; //备份将被删除的元素
    remove(r, r + 1);     //调用区间删，等效为对区间[r, r + 1)的删除
    return re_elem;       //反回被删除的元素
}

//去重
//无序向量去重
template <typename T>
int Vector<T>::deduplicate()
{
    int oldSize = _size; //记录原始规模
    Rank i = 1;
    while (i < _size)
    {                                //从前向后依次检查_elem[i]
        (find(_elem[i], 0, i) < 0) ? //在其前缀中寻找相同元素
            i++
                                   : remove(i); //若查找到，删除该元素并检查其后继元素
    }
    return oldSize - _size; //返回删除的元素个数
}

//有序向量去重
template <typename T>
int Vector<T>::uniquify()
{
    int i = 0, j = 0;
    while (++j < _size)
    { //逐一扫描，直至末元素
        if (_elem[i] != _elem[j])
        {                          //跳过雷同元素
            _elem[++i] = _elem[j]; //发现不同元素时，向前移至紧邻于前者右侧
        }
    }
    _size = ++i;
    shrink(); //直接截去尾部多余的元素
    return j - i;
}

//向量的遍历接口
// 方法一：
template <typename T>
void Vector<T>::traverse(void (*visit)(T &))
{ // 借助函数指针机制
    for (int i = 0; i < _size; i++)
    {
        visit(_elem[i]);
    }
}

// 方法二：
template <typename T>   // 元素类型
template <typename VST> // 操作器
void Vector<T>::traverse(VST &visit)
{ // 借助函数对象机制
    for (int i = 0; i < _size; i++)
    {
        visit(_elem[i]); // 遍历变量
    }
}

/*
template <typename T, typename VST>
void traverse(VST &visit, T &V, Rank lo, Rank hi)
{
    for (Rank i = lo; i <= hi; i++)
    {
        visit(V[i]);
    }
}
*/

//输出操作器的实现
void show(int e)
{
    cout << e << " ";
}

//排序
//冒泡排序
//冒泡扫描交换
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
    while (!bubble(lo, hi--))
        ;
} //逐趟做扫描交换，直至全序

//冒泡排序
template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi)
{
    bool sorted = true; //整体有序标志
    while (++lo < hi)
    { //自左向右，逐一检查各对相邻元素
        if (_elem[lo - 1] > _elem[lo])
        { //若逆序
            sorted = false;
            swap(_elem[lo - 1], _elem[lo]); //交换
        }
    }
    return sorted; //返回有序标志
}

//归并排序
//分治策略
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
    if (hi - lo < 2)
        return;
    int mi = (lo + hi) >> 1;
    mergeSort(lo, mi); //对前半区间排序
    mergeSort(mi, hi); //对后半区间排序
    merge(lo, mi, hi); //两个区间的归并
}
//归并的实现
template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
    T *A = _elem + lo;
    int lb = mi - lo;
    T *B = new T[lb];
    for (Rank i = 0; i < lb; i++)
        B[i] = A[i];
    int lc = hi - mi;
    T *C = _elem + mi;
    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)
    {
        if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
            A[i++] = B[j++];
        if ((k < lc) && (!(j < lb) || (C[k] < B[j])))
            A[i++] = C[k++];
    }
    delete[] B;
}
//以上为Vector构造

//计算行列式
double cal_det(int n, Vector<Vector<double>> a)
{
    if (n == 2)
        return a[0][0] * a[1][1] - a[0][1] * a[1][0];
    else
    {
        int sum = 0;
        for (int k = 0; k < n; k++)
        {
            Vector<Vector<double>> b;
            for (int i = 1; i < n; i++)
            {
                Vector<double> c;
                for (int j = 0; j < n; j++)
                {
                    if (j == k)
                        continue;
                    c.insert(a[i][j]);
                }
                b.insert(c);
            }
            if (k % 2 == 0)
            {
                sum += a[0][k] * cal_det(n - 1, b);
            }
            else
                sum -= a[0][k] * cal_det(n - 1, b);
        }
        return sum;
    }
}

int main()
{
    int n;
    double x;
    Vector<Vector<double>> a;
    cin >> n;
    if (n <= 0)
        cout << "error: wrong input!" << endl;
    else if (n >= 9)
        cout << "not support calculation!" << endl;
    else if (n == 1)
    {
        cin >> x;
        cout << x << endl;
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            Vector<double> b;
            for (int j = 0; j < n; j++)
            {
                cin >> x;
                b.insert(x);
            }
            a.insert(b);
        }
        cout << cal_det(n, a) << endl;
    }
}
