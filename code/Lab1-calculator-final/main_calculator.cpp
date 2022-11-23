#include <iostream>
#include <algorithm>
#include <math.h>
#include <string>
using namespace std;

//以下为Vector构造
typedef int Rank;          //定义秩
#define DEFAULT_CAPACITY 3 //默认初始容量
int mode;

//函数的存储与定义
struct Func
{
    string ori_fun;         //输入的函数
    string expression;      //表达式部分
    string afterchange_exp; //经过代入数据可计算的表达式
    string name;            //函数名
    string var;             //变量
};

//————————————————————————————以下为vector和stack的构造——————————————————————————————
template <typename T>
class Vector
{
public:
    //基本成员
    Rank _size;    //元素个数
    int _capacity; //实际空间
    T *_elem;      //元素指针

    //其他内部函数
    void copyFrom(T const *A, Rank lo, Rank hi); //从A中复制区间[lo, hi)
    void expand();                               //空间不足时扩容
    void shrink();                               //装填因子过小时压缩空间

    //构造函数

    Vector(int c = DEFAULT_CAPACITY, int s = 0)
    { //默认构造
        _elem = new T[_capacity = 30];
        _size = 0;
    }
    //复制构造的时候注意不要把string置0

    //复制构造接口

    Vector(T const *A, Rank n) { copyFrom(A, 0, n); }                           //从数组复制
    Vector(T const *A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }               //复制数组区间
    Vector(Vector<T> const &V) { copyFrom(V._elem, 0, V._size); }               //拷贝构造
    Vector(Vector<T> const &V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //复制向量区间

    //析构函数
    //~Vector();

    ~Vector()
    {
        delete[] _elem;
    } //删除数组

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

// stack模板类
template <typename T>
class Stack : public Vector<T>
{
public:
    void push(T const &e) { this->insert(e); }
    T pop() { return this->remove(this->_size - 1); }
    T &top() { return (*this)[this->_size - 1]; }
};

//———————————————————————以上为vector和stack的构造————————————————————————————————

//判断是否是字符
bool isdigit(char c)
{
    return (c >= '0' && c <= '9');
}

// string的字符转成double数字（使用std::stod）
double get_op(const string &expression, int &pos)
{
    string operand = "";
    while (expression[pos] == '.' || isdigit(expression[pos]))
        operand.push_back(expression[pos++]);

    double num = stod(operand);
    return num;
}

//后缀表达式求值
double cal_arithmetic(string str)
{
    int len = str.length();
    int pos = 0;
    char c;
    Stack<double> num_stack;

    //遍历
    while (pos < len)
    {
        if (isdigit(str[pos]))
        {
            double op = get_op(str, pos);
            num_stack.push(op);
        }
        else if (str[pos] == ' ')
            pos++;
        else
        {
            double num1, num2;
            c = str[pos++];
            num2 = num_stack.top();
            num_stack.pop();
            num1 = num_stack.top();
            num_stack.pop();

            switch (c)
            {
            case '+':
                num_stack.push(num1 + num2);
                break;
            case '-':
                num_stack.push(num1 - num2);
                break;
            case '*':
                num_stack.push(num1 * num2);
                break;
            case '/':
                num_stack.push(num1 / num2);
                break;
            case '^':
                num_stack.push(pow(num1, num2));
            }
        }
    }
    double ans = num_stack.top();
    return ans;
}

//对负号进行预处理，在负号前面加0, check!
void tran_nagative_sign(string &str)
{
    int len = str.length();
    int pos = 0;
    while (pos < len)
    {
        // cout << pos << endl;
        if (pos == 0 && str[pos] == '-')
        {
            str.insert(pos, 1, '0');
            pos++;
        }
        else if (str[pos] == '-' && str[pos - 1] == '(')
        {
            str.insert(pos, 1, '0');
            pos++;
        }
        else
            pos++;
    }
    return;
}

//中缀表达式 转 后缀表达式
// stack<string>的构造问题解决了
string to_postfix(string str)
{
    // cout << 22222 << endl;
    Stack<string> num_stack, operator_stack;
    // cout << 3 << endl;
    int len = str.length();
    int pos = 0;

    while (pos < len)
    {
        if (isdigit(str[pos]))
        {
            double op = get_op(str, pos);
            num_stack.push(to_string(op)); // to_string:将数字常量转换为字符串
        }
        else if (str[pos] == ' ')
            pos++;
        else
        {
            char ch = str[pos++];
            if (ch == '+')
            {
                while (!operator_stack.empty() && operator_stack.top() == "^")
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                while (!operator_stack.empty() && (operator_stack.top() == "*" || operator_stack.top() == "/"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                while (!operator_stack.empty() && (operator_stack.top() == "+" || operator_stack.top() == "-"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                operator_stack.push("+");
            }
            else if (ch == '-')
            {
                while (!operator_stack.empty() && operator_stack.top() == "^")
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                while (!operator_stack.empty() && (operator_stack.top() == "*" || operator_stack.top() == "/"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                while (!operator_stack.empty() && (operator_stack.top() == "+" || operator_stack.top() == "-"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                operator_stack.push("-");
            }
            else if (ch == '*')
            {
                while (!operator_stack.empty() && operator_stack.top() == "^")
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                while (!operator_stack.empty() && (operator_stack.top() == "*" || operator_stack.top() == "/"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                operator_stack.push("*");
            }
            else if (ch == '/')
            {
                while (!operator_stack.empty() && operator_stack.top() == "^")
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                while (!operator_stack.empty() && (operator_stack.top() == "*" || operator_stack.top() == "/"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                operator_stack.push("/");
            }
            else if (ch == '^')
            {
                while (!operator_stack.empty() && operator_stack.top() == "^")
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                operator_stack.push("^");
            }
            else if (ch == '(')
                operator_stack.push("(");
            else
            {
                while (operator_stack.top() != "(")
                {
                    num_stack.push(operator_stack.top());
                    operator_stack.pop();
                }
                operator_stack.pop();
            }
        }
    }
    while (!operator_stack.empty())
    {
        num_stack.push(operator_stack.top());
        operator_stack.pop();
    }

    string ans = "";
    while (!num_stack.empty())
        ans = num_stack.top() + " " + ans, num_stack.pop();

    return ans;
}

//表达式括号匹配检查,可检查[{(, check！
bool check_paren(const char exp[], int lo, int hi)
{
    Stack<char> S;
    for (int i = lo; i <= hi; i++)
    {
        switch (exp[i])
        {
        case '(':
        case '[':
        case '{':
            S.push(exp[i]);
            break;
        case ')':
            if ((S.empty()) || ('(' != S.pop()))
                return false;
            break;
        case ']':
            if ((S.empty()) || ('[' != S.pop()))
                return false;
            break;
        case '}':
            if ((S.empty()) || ('{' != S.pop()))
                return false;
            break;
        default:
            break;
        }
    }
    return S.empty();
}

// 检测操作符与数字的组合是否合法
bool check_symbol(string str)
{
    int num_sum = 0; //数字总数
    int sym_sum = 0; //符号总数

    int pos = 0;
    int len = str.length();

    //统计符号总数
    while (pos < len)
    {
        if (str[pos] == '+' || str[pos] == '-' || str[pos] == '*' || str[pos] == '/' || str[pos] == '^')
        {
            pos++;
            sym_sum++;
        }
        else
            pos++;
    }
    // cout << sym_sum << endl;

    //统计数字总数
    Stack<double> num_stack;
    pos = 0;
    while (pos < len)
    {
        if (isdigit(str[pos]))
        {
            double op = get_op(str, pos);
            num_stack.push(op); // to_string:将数字常量转换为字符串
            // cout << pos << endl;
            num_sum++;
        }
        else if (str[pos] == '(' || str[pos] == ')' || str[pos] == ' ' || str[pos] == '+' || str[pos] == '-' || str[pos] == '*' || str[pos] == '/' || str[pos] == '^')
            pos++;
    }

    // cout << num_sum << endl;

    if (num_sum - sym_sum == 1)
        return 1;
    else
        return 0;
}

//——————————————————————————以上为表达式求值部分——————————————————————————————————————
// 1.输入数字表达式可以求值
// 2.可以检查括号配对
// TODO:检测是否有连续的操作符，强制报错

//—————————————————————————以下为函数和单变量表达式求值——————————————————————————————
//函数中获取要求的值
double get_key(string str)
{
    double key;

    int len = str.length();
    int hi;
    int lo;
    int pos = 0;

    //找左括号
    while (1)
    {
        if (str[pos] == '(')
        {
            lo = pos;
            break;
        }
        else
            pos++;
    }
    //找右括号
    pos = 0;
    while (1)
    {
        if (str[pos] == ')')
        {
            hi = pos;
            break;
        }
        else
            pos++;
    }
    int x = 0;

    string fun(str, lo + 1, hi - lo - 1);
    key = get_op(fun, x);

    return key;
}

//函数：获取要求的函数表达式
string get_function(string str)
{

    int len = str.length();
    int pos = 0;
    while (pos < len)
    {
        if (str[pos] != '=')
            pos++;
        else
            break;
    }
    while (str[pos + 1] == ' ')
        pos++;
    string fun(str, pos + 1, len - pos - 1);
    return fun;
}

//函数：获取函数的名字(类似找变量)
string fun_get_name(string str)
{
    int i = 0;
    int lo = 0;
    int hi = 0;
    int flag = 0;
    int len = str.length();

    string wrong_str = "wrongstring";
    if (!((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z') || str[0] == '_'))
        return wrong_str;
    while (lo < len && flag == 0 && i < len)
    {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '_')
        {
            lo = i;
            i++;
            while (1)
            {
                if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^' || str[i] == ' ' || str[i] == ')' || str[i] == '(')
                {
                    hi = i;
                    flag = 1;
                    break;
                }
                i++;
            }
        }
        else
            i++;
    }

    if (flag == 0)
        return wrong_str;
    else
    {
        string var(str, lo, hi - lo);
        return var;
    }
}

//函数：在表达式中找到变量
string fun_get_variable(string str)
{
    int i = 0;
    int lo = 0;
    int hi = 0;
    int len = str.length();
    while (lo < len)
    {
        if (str[i] == '(')
        {
            lo = i;
            i++;
        }
        else if (str[i] == ')')
        {
            hi = i;
            break;
        }
        else
            i++;
    }
    string var(str, lo + 1, hi - lo - 1);

    return var;
}

//表达式：把表达式中的变量换成要求的值
void change_str(string &str, string var, double key)
{
    int lo = 0;
    int hi = 0;
    int len = var.length();
    int flag = 0;
    string strkey = to_string(key);
    //负号特判
    if (key < 0)
    {
        strkey = '(' + strkey + ')';
    }
    while ((flag = str.find(var, flag)) != string::npos)
    {
        flag = str.find(var);
        if (flag != -1)
        {
            str.replace(flag, len, strkey);
        }
    }
}

//函数：把表达式中的变量换成要求的值,不改变原式
string const_change_str(string str, string var, double key)
{
    string change_string(str);
    int lo = 0;
    int hi = 0;
    int len = var.length();
    int flag = 0;
    string strkey = to_string(key);
    while ((flag = change_string.find(var, flag)) != string::npos)
    {
        flag = change_string.find(var);
        if (flag != -1)
        {
            change_string.replace(flag, len, strkey);
        }
    }
    return change_string;
}

//单变量表达式：在表达式中找到变量
string get_variable(string str)
{
    int i = 0;
    int lo = 0;
    int hi = 0;
    int len = str.length();

    str += "  ";

    int flag = 0;
    while (lo < len && flag == 0 && i < len)
    {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '_')
        {
            lo = i;
            i++;
            while (1)
            {
                if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '^' || str[i] == ' ' || str[i] == ')')
                {
                    hi = i;
                    flag = 1;
                    break;
                }
                i++;
            }
        }
        else
            i++;
    }
    string wrong_str = "wrongstring";
    if (flag == 0)
        return wrong_str;
    else
    {
        string var(str, lo, hi - lo);
        return var;
    }
}
//—————————————————————————以上为函数和单变量表达式求值——————————————————————————————
// TODO: 函数和单变量表达式的 输入合法性检查。。。

//——————————————————————————————————一元多项式操作————————————————————————————————
//多项式输出
void output_poly(Vector<double> poly_ans, int times)
{
    int flag = 0;
    cout << "Answer:";
    if (poly_ans[0] != 0)
    {
        cout << poly_ans[0];
        flag = 1;
    }
    for (int i = 1; i <= times; i++)
    {
        if (poly_ans[i] == 0)
            continue;

        else if (poly_ans[i] > 0)
        {
            if (i == 1 && flag == 0)
            {
                if (poly_ans[i] == 1)
                    cout << "x";
                else
                    cout << poly_ans[i] << "x";
                flag = 1;
            }
            else if (i == 1 && flag == 1)
            {
                if (poly_ans[i] == 1)
                    cout << "+x";
                else
                    cout << "+" << poly_ans[i] << "x";
            }
            else if (i != 1 && flag == 0)
            {
                if (poly_ans[i] == 1)
                    cout << "x^" << i;
                else
                    cout << poly_ans[i] << "x^" << i;
                flag = 1;
            }
            else if (i != 1 && flag == 1)
            {
                if (poly_ans[i] == 1)
                    cout << "+x^" << i;
                else
                    cout << "+" << poly_ans[i] << "x^" << i;
            }
        }
        else if (poly_ans[i] < 0)
        {
            if (i == 1)
            {
                if (poly_ans[i] == -1)
                    cout << "-x";
                else
                    cout << poly_ans[i] << "x";
            }
            else
            {
                if (poly_ans[i] == -1)
                    cout << "-x^" << i;
                else
                    cout << poly_ans[i] << "x^" << i;
            }
        }
    }
    if (flag == 0)
        cout << '0';
    cout << endl;
}
//多项式加法
void poly_plus(Vector<double> &poly1_item, Vector<double> &poly2_item, int t1, int t2)
{
    int times = max(t1, t2);
    Vector<double> poly_ans;

    //加法运算
    for (int i = 0; i <= times; i++)
    {
        poly_ans.insert(poly1_item[i] + poly2_item[i]);
    }
    //多项式输出
    output_poly(poly_ans, times);
}
//多项式减法
void poly_minus(Vector<double> &poly1_item, Vector<double> &poly2_item, int t1, int t2)
{
    int times = max(t1, t2);
    Vector<double> poly_ans;
    //减法运算
    for (int i = 0; i <= times; i++)
    {
        poly_ans.insert(poly1_item[i] - poly2_item[i]);
    }
    //多项式输出
    output_poly(poly_ans, times);
}
//多项式乘法
void poly_multi(Vector<double> &poly1_item, Vector<double> &poly2_item, int t1, int t2)
{
    int times = t1 + t2;
    Vector<double> poly_ans;
    double multi[30][30] = {0};
    double ans[100] = {0};
    //乘法运算
    for (int i = 0; i <= t1; i++)
    {
        for (int j = 0; j <= t2; j++)
        {
            multi[i][j] = poly1_item[i] * poly2_item[j];
        }
    }
    ans[0] = multi[0][0];
    for (int k = 1; k <= times; k++)
    {
        for (int i = 0, j = k; j >= 0; i++, j--)
        {
            ans[k] += multi[i][j];
        }
    }
    for (int k = 0; k <= times; k++)
    {
        poly_ans.insert(ans[k]);
    }

    //多项式输出
    output_poly(poly_ans, times);
}
//多项式积分
void poly_integral(Vector<double> &poly_item, int t)
{
    int times = t + 1;
    double item[50] = {0};
    Vector<double> poly_ans;
    //积分运算
    for (int i = 0; i <= t; i++)
    {
        item[i + 1] = poly_item[i] / (i + 1);
    }
    poly_ans.insert(0);
    for (int i = 1; i <= times; i++)
    {
        poly_ans.insert(item[i]);
    }
    //多项式输出
    output_poly(poly_ans, times);
    return;
}

//多项式微分
void poly_differential(Vector<double> &poly_item, int t)
{
    int times = t - 1;
    double item[50] = {0};
    Vector<double> poly_ans;
    //积分运算
    for (int i = 1; i <= t; i++)
    {
        item[i - 1] = poly_item[i] * i;
        // cout << item[i - 1] << endl;
    }
    for (int i = 0; i <= times; i++)
    {
        poly_ans.insert(item[i]);
    }
    //多项式输出
    output_poly(poly_ans, times);
    return;
}

//——————————————————————————————————一元多项式操作—————————————————————————————————————

//————————————————————————————————矩阵操作————————————————————————————————————————
//答案矩阵
Vector<Vector<double>> ans;
//矩阵输出
void output_mat(int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << ans[i][j] << " ";
        }
        cout << endl;
    }
    int x = ans.size();
    ans.remove(0, x);
}
//矩阵相加
void mat_add(Vector<Vector<double>> mat1, Vector<Vector<double>> mat2, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        Vector<double> a;
        for (int j = 0; j < m; j++)
        {
            a.insert(mat1[i][j] + mat2[i][j]);
        }
        ans.insert(a);
    }
    return;
}
//矩阵相减
void mat_minus(Vector<Vector<double>> mat1, Vector<Vector<double>> mat2, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        Vector<double> a;
        for (int j = 0; j < m; j++)
        {
            a.insert(mat1[i][j] - mat2[i][j]);
        }
        ans.insert(a);
    }
    return;
}
//矩阵相乘
void mat_multi(Vector<Vector<double>> mat1, Vector<Vector<double>> mat2, int n1, int m1, int n2, int m2)
{
    double matrix[30][30] = {0};
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            for (int k = 0; k < m1; k++)
            {
                matrix[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
        // cout<<endl;
    }
    for (int i = 0; i < n1; i++)
    {
        Vector<double> a;
        for (int j = 0; j < m2; j++)
        {
            a.insert(matrix[i][j]);
        }
        ans.insert(a);
    }
    return;
}

//矩阵转置
void mat_tran(Vector<Vector<double>> mat, int n1, int m1)
{
    double matrix[30][30] = {0};
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m1; j++)
        {
            matrix[j][i] = mat[i][j];
        }
        // cout<<endl;
    }
    for (int i = 0; i < m1; i++)
    {
        Vector<double> a;
        for (int j = 0; j < n1; j++)
        {
            a.insert(matrix[i][j]);
        }
        ans.insert(a);
    }
    return;
}
//计算行列式
double mat_cal_det(int n, Vector<Vector<double>> a)
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
                sum += a[0][k] * mat_cal_det(n - 1, b);
            }
            else
                sum -= a[0][k] * mat_cal_det(n - 1, b);
        }
        return sum;
    }
}
//————————————————————————————————矩阵操作————————————————————————————————————————

//————————————————————————————————主函数——————————————————————————————————————————————
int main()
{

    while (1)
    {
        cout << "please input the mode of the calculator:" << endl;
        cout << "0:表达式求值;" << endl;
        cout << "1:一元多项式的+-*计算;" << endl;
        cout << "2:一元多项式的积分和求导计算;" << endl;
        cout << "3:矩阵运算;" << endl;
        cout << "4:单变量表达式求值;" << endl;
        cout << "5:单变量函数求值;" << endl;
        cout << "-1:退出;" << endl;

        cin >> mode;
        getchar();

        //退出————————————————————————————————————————————————————————————————————————
        if (mode == -1)
        {
            cout << "exit the calculator..." << endl;
            break;
        }

        //四则运算求值—————————————————————————————————————————————————————————————————
        else if (mode == 0)
        {
            cout << "请输入表达式：" << endl;
            string str;

            getline(cin, str);

            //表达式检查，如果有变量，不合法
            string variable = get_variable(str);
            if (variable != "wrongstring")
            {
                cout << "Illegal input, not all numbers!!" << endl;
                continue;
            }

            //表达式检查和求值————————————————————————
            int flag = 1;
            int len = str.length();
            char c[len + 2];
            for (int i = 0; i < sizeof(c); i++)
            {
                c[i] = str[i];
            }
            //检查括号配对:0不合法，1合法
            flag = check_paren(c, 0, len);
            //负号预处理
            tran_nagative_sign(str);
            // cout << str << endl;

            flag = check_symbol(str);

            if (flag == 0)
            {
                cout << "Illegal input!!" << endl;
            }
            else
            {
                cout << "Answer:" << cal_arithmetic(to_postfix(str)) << endl;
            }
            //————————————————————————————————————————
        }

        //一元多项式运算（+ - *）——————————————————————————————————————————————————————
        else if (mode == 1)
        {
            string poly1;
            string poly2;
            int t1, t2, op;
            cout << "Please enter the highest time of the first polynomial:" << endl;
            cin >> t1;
            cout << "please input the coefficient of the first polynomial:" << endl;
            cout << "(for example:4 3 4 0 5 represent 4+3x+4x^2+5x^4)" << endl;
            Vector<double> poly1_item;
            for (int i = 1; i <= t1 + 1; i++)
            {
                double x;
                cin >> x;
                poly1_item.insert(x);
            }

            cout << "Please enter the highest time of the second polynomial:" << endl;
            cin >> t2;
            cout << "please input the coefficient of the second polynomial:" << endl;
            Vector<double> poly2_item;
            for (int i = 0; i <= t2; i++)
            {
                double x;
                cin >> x;
                poly2_item.insert(x);
            }

            cout << "please input the the operation(1:+  2:-  3:*):" << endl;
            cin >> op;

            //+
            if (op == 1)
            {
                poly_plus(poly1_item, poly2_item, t1, t2);
            }

            //-
            else if (op == 2)
            {
                poly_minus(poly1_item, poly2_item, t1, t2);
            }

            //*
            else if (op == 3)
            {
                poly_multi(poly1_item, poly2_item, t1, t2);
            }

            //非法输入
            else
                cout << "Illegal operation!" << endl;
        }

        //一元多项式运算（微分、积分）——————————————————————————————————————————————————
        else if (mode == 2)
        {
            string poly;
            int t, op;
            cout << "Please enter the highest time of the polynomial:" << endl;
            cin >> t;
            cout << "input the coefficient of the polynomial:" << endl;
            cout << "(for example:4 3 4 0 5 represent 4+3x+4x^2+5x^4)" << endl;
            Vector<double> poly_item;
            for (int i = 1; i <= t + 1; i++)
            {
                double x;
                cin >> x;
                poly_item.insert(x);
            }
            cout << "choose the operation : 1 integral; 2 differential" << endl;
            cin >> op;
            if (op == 1)
            {
                poly_integral(poly_item, t);
            }
            else if (op == 2)
            {
                poly_differential(poly_item, t);
            }
            else
                cout << "Illegal input!" << endl;
        }

        //矩阵运算—————————————————————————————————————————————————————————————————————
        else if (mode == 3)
        {
            int op;
            cout << "enter operation : 1 plus; 2 minus; 3 multiply; 4 transpose; 5 determinant" << endl;
            cin >> op;

            //相加
            if (op == 1)
            {
                int n1, m1;
                double x;
                cout << "输入两个矩阵的阶数n×m:" << endl;
                cin >> n1 >> m1;
                if (n1 <= 0 || m1 <= 0)
                    cout << "error: wrong input!" << endl;
                else
                {
                    Vector<Vector<double>> mat1;
                    Vector<Vector<double>> mat2;
                    cout << "输入第一个矩阵" << endl;
                    for (int i = 0; i < n1; i++)
                    {
                        Vector<double> v1;
                        for (int j = 0; j < m1; j++)
                        {
                            cin >> x;
                            v1.insert(x);
                        }
                        mat1.insert(v1);
                    }

                    cout << "输入第二个矩阵" << endl;
                    for (int i = 0; i < n1; i++)
                    {
                        Vector<double> v2;
                        for (int j = 0; j < m1; j++)
                        {
                            cin >> x;
                            v2.insert(x);
                        }
                        mat2.insert(v2);
                    }
                    mat_add(mat1, mat2, n1, m1);
                    cout << "Answer:" << endl;
                    output_mat(n1, m1);
                }
            }

            //相减
            else if (op == 2)
            {
                int n1, m1;
                double x;
                cout << "输入两个矩阵的阶数n×m:" << endl;
                cin >> n1 >> m1;
                if (n1 <= 0 || m1 <= 0)
                    cout << "error: wrong input!" << endl;
                else
                {
                    Vector<Vector<double>> mat1;
                    Vector<Vector<double>> mat2;
                    cout << "输入第一个矩阵" << endl;
                    for (int i = 0; i < n1; i++)
                    {
                        Vector<double> v1;
                        for (int j = 0; j < m1; j++)
                        {
                            cin >> x;
                            v1.insert(x);
                        }
                        mat1.insert(v1);
                    }

                    cout << "输入第二个矩阵" << endl;
                    for (int i = 0; i < n1; i++)
                    {
                        Vector<double> v2;
                        for (int j = 0; j < m1; j++)
                        {
                            cin >> x;
                            v2.insert(x);
                        }
                        mat2.insert(v2);
                    }
                    mat_minus(mat1, mat2, n1, m1);
                    cout << "Answer:" << endl;
                    output_mat(n1, m1);
                }
            }

            //相乘
            else if (op == 3)
            {
                int n1, m1, n2, m2;
                double x;
                cout << "输入第一个矩阵的阶数n×m:" << endl;
                cin >> n1 >> m1;
                cout << "输入第二个矩阵的阶数n×m:" << endl;
                cin >> n2 >> m2;

                if (n1 <= 0 || m1 <= 0 || n2 <= 0 || m2 <= 0 || m1 != n2)
                    cout << "error: wrong input!" << endl;
                else
                {
                    Vector<Vector<double>> mat1;
                    Vector<Vector<double>> mat2;
                    cout << "输入第一个矩阵" << endl;
                    for (int i = 0; i < n1; i++)
                    {
                        Vector<double> v1;
                        for (int j = 0; j < m1; j++)
                        {
                            cin >> x;
                            v1.insert(x);
                        }
                        mat1.insert(v1);
                    }

                    cout << "输入第二个矩阵" << endl;
                    for (int i = 0; i < n2; i++)
                    {
                        Vector<double> v2;
                        for (int j = 0; j < m2; j++)
                        {
                            cin >> x;
                            v2.insert(x);
                        }
                        mat2.insert(v2);
                    }
                    mat_multi(mat1, mat2, n1, m1, n2, m2);
                    cout << "Answer:" << endl;
                    output_mat(n1, m2);
                }
            }
            else if (op == 4)
            {
                int n, m;
                double x;
                cout << "输入矩阵阶数：n×m" << endl;
                cin >> n >> m;
                // 0阶 不支持
                if (n <= 0 || m <= 0)
                    cout << "error: wrong input!" << endl;
                //太高阶 不支持
                else if (n >= 15 || m >= 15)
                    cout << "not support calculation!" << endl;
                // 1阶
                else if (n == 1 && m == 1)
                {
                    cout << "请输入矩阵" << endl;
                    cin >> x;
                    cout << "Answer:" << x << endl;
                }
                else
                {
                    cout << "请输入矩阵" << endl;
                    Vector<Vector<double>> a;
                    for (int i = 0; i < n; i++)
                    {
                        Vector<double> b;
                        for (int j = 0; j < m; j++)
                        {
                            cin >> x;
                            b.insert(x);
                        }
                        a.insert(b);
                    }
                    mat_tran(a, n, m);
                    cout << "Answer:" << endl;
                    output_mat(m, n);
                }
            }

            //求行列式
            else if (op == 5)
            {
                int n;
                double x;
                cout << "输入矩阵阶数：" << endl;
                cin >> n;
                // 0阶 不支持
                if (n <= 0)
                    cout << "error: wrong input!" << endl;
                //太高阶 不支持
                else if (n >= 15)
                    cout << "not support calculation!" << endl;
                // 1阶
                else if (n == 1)
                {
                    cin >> x;
                    cout << "Answer:" << x << endl;
                }
                else
                {
                    Vector<Vector<double>> a;
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
                    cout << "Answer: " << mat_cal_det(n, a) << endl;
                }
            }
        }

        // 单变量表达式求值—————————————————————————————————————————————————————————————
        else if (mode == 4)
        {
            //输入单变量表达式
            string fun1;
            cout << "please input your function: (example:3+4*X2)" << endl;
            getline(cin, fun1);

            //输入要求的值
            string run1;
            double key;
            cout << "please input the key:(example:if you want to get x=5, please input: 5)" << endl;
            cin >> key;

            //在表达式中找到变量

            string variable1 = get_variable(fun1);

            //没有变量时报错
            if (variable1 == "wrongstring")
            {
                cout << "Illegal input: no variable!!" << endl;
                continue;
            }
            cout << "The variable is:" << variable1 << endl;

            //把表达式中的变量换成要求的值（代入）
            change_str(fun1, variable1, key);

            //变量多于一个时报错
            string variable2 = get_variable(fun1);
            if (variable2 != "wrongstring")
            {
                cout << "Illegal input: too many variables!!" << endl;
                continue;
            }

            //表达式检查和求值————————————————————————
            int flag = 1;
            int len = fun1.length();
            char c[len + 2];
            for (int i = 0; i < sizeof(c); i++)
            {
                c[i] = fun1[i];
            }
            //检查括号配对
            flag = check_paren(c, 0, len);
            //负号预处理
            tran_nagative_sign(fun1);
            //检查连续出现的符号
            flag = check_symbol(fun1);

            if (flag == 0)
            {
                cout << "Illegal input!!" << endl;
            }
            else
            {
                cout << "Answer:" << cal_arithmetic(to_postfix(fun1)) << endl;
            }
            //————————————————————————————————————————
        }

        // 单变量函数求值——————————————————————————————————————————————————————————
        else if (mode == 5)
        {
            int wrong = 0;

            //定义函数
            Func function[100];
            int num = 0;

            //输入函数表达式
            string op;
            cout << "If you want to define a function, please input DEF" << endl;
            cout << "If you want to calculate a function, please input RUN" << endl;
            cout << "If you want to exit, please input EXIT to exit this mode" << endl;
            cout << "CAUTION:If your input is illegal, you will exit this mode with all memory disappeared!" << endl;
            getline(cin, op);

            //不规范操作输入
            if (op != "DEF" && op != "RUN" && op != "EXIT")
            {
                cout << "Illegal input!! exit this mode..." << endl;
                continue;
            }
            //规范操作输入
            while (op == "DEF" || op == "RUN" || op == "EXIT")
            {
                //定义新函数
                if (op == "DEF")
                {
                    num++;
                    cout << "please input the function, example: f(X2)=3+4*X2" << endl;
                    string fun1;

                    //输入函数如 f(X2)=3+4*X2
                    getline(cin, fun1);
                    function[num].ori_fun = fun1;

                    //在表达式中（括号里）找到变量
                    string variable = fun_get_variable(fun1);
                    function[num].var = variable;
                    // cout << "The variable is:" << variable << endl;

                    //在表达式中找到函数名
                    function[num].name = fun_get_name(fun1);
                    cout << "The name is:" << function[num].name << endl;

                    //在表达式中找到要求的表达式部分
                    function[num].expression = get_function(fun1);
                    // cout << "function[num].expression" << function[num].expression << endl;
                }

                //运行旧函数
                else if (op == "RUN")
                {
                    string run_fun;
                    cout << "please input the key:example:f(1)" << endl;
                    getline(cin, run_fun);

                    //在公式中找到函数名
                    string now_name = fun_get_name(run_fun);
                    cout << "The name is:" << now_name << endl;

                    //在函数记忆中检索该函数名
                    //如果函数重名，优先采用后定义的函数！！！！！
                    int i;
                    for (i = num; i >= 1; i--)
                    {
                        //检索到了，开始计算
                        if (now_name == function[i].name)
                        {
                            //找到要求的值
                            // cout << "this is i" << i << endl;

                            double key = get_key(run_fun);
                            // cout << "the key is: " << key << endl;

                            //把表达式中的变量换成要求的值（代入）
                            function[i].afterchange_exp = const_change_str(function[i].expression, function[i].var, key);
                            // cout << "function[i].afterchange_exp: " << function[i].afterchange_exp << endl;

                            //表达式检查和求值————————————————————————
                            int flag = 1;
                            int len = function[i].afterchange_exp.length();
                            char c[len + 2];
                            for (int j = 0; j < sizeof(c); j++)
                            {
                                c[j] = function[i].afterchange_exp[j];
                            }
                            //检查括号配对
                            flag = check_paren(c, 0, len);
                            //负号预处理
                            tran_nagative_sign(function[i].afterchange_exp);
                            //检查连续出现的符号
                            flag = check_symbol(function[i].afterchange_exp);

                            if (flag == 0)
                            {
                                cout << "Illegal input!!" << endl;
                            }
                            else
                            {
                                cout << "Answer:" << cal_arithmetic(to_postfix(function[i].afterchange_exp)) << endl;
                            }
                            //————————————————————————————————————————

                            //算完退出
                            break;
                        }
                    }
                    //函数未定义
                    if (i == 0)
                    {
                        cout << "The function have not been defined yet!" << endl;
                        cout << endl;
                    }
                }

                //退出
                else if (op == "EXIT")
                {
                    cout << "exit this mode..." << endl;
                    wrong = 1; // wrong = 1代表正常退出
                    break;
                }

                //输入下一步操作
                cout << "if you want to define, please input DEF" << endl;
                cout << "if you want to calculate, please input RUN" << endl;
                cout << "if you want to exit, please input EXIT to exit this mode" << endl;
                cout << "CAUTION:If your input is illegal, you will exit this mode with all memory disappeared!" << endl;
                getline(cin, op);
            }

            if (wrong == 0)
            {
                cout << "Illegal input!! exit this mode..." << endl;
            }

            /* 旧版函数操作
            //输入要求的值
            string run1;
            double key;
            cout << "please input the key:(example:if you want to get f(5), please input: 5)" << endl;
            cin >> key;

            //在表达式中找到变量
            string variable = fun_get_variable(fun1);
            // cout << "The variable is:" << variable << endl;

            //在表达式中找到要求的函数表达式
            string function = get_function(fun1);

            // cout << function << endl;
            //把表达式中的变量换成要求的值（代入）
            change_str(function, variable, key);

            //表达式检查和求值————————————————————————
            int flag = 1;
            int len = function.length();
            char c[len + 2];
            for (int i = 0; i < sizeof(c); i++)
            {
                c[i] = function[i];
            }
            //检查括号配对
            flag = check_paren(c, 0, len);
            //负号预处理
            tran_nagative_sign(function);
            //检查连续出现的符号
            flag = check_symbol(function);

            if (flag == 0)
            {
                cout << "Illegal input!!" << endl;
            }
            else
            {
                cout << "Answer:" << cal_arithmetic(to_postfix(function)) << endl;
            }
            //————————————————————————————————————————
            */
        }
        cout << endl;
    }

    return 0;
}