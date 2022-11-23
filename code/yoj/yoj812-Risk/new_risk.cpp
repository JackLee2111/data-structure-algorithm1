#include <stdio.h>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;
int binSearch(int *A, int const &e, int lo, int hi)
{
    while (lo < hi)
    {                                        //每步迭代仅需做一次比较判断，有两个分支
        int mi = (lo + hi) >> 1;             //以中点为轴点
        (e < A[mi]) ? hi = mi : lo = mi + 1; //经比较后确定深入[lo, mi)或(mi, hi)
    }                                        //成功查找不能提前终止

    return --lo; //循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
} //有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置

void merge(vector<int> &data, int lo, int mi, int hi)
{

    int *A = data.data() + lo;
    int lb = mi - lo;
    int *B = new int[lb];

    for (int i = 0; i < lb; i++)
    {
        B[i] = A[i];
    }
    int lc = hi - mi;
    int *C = data.data() + mi;
    for (int i = 0, j = 0, k = 0; (j < lb) || (k < lc);)
    {
        if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
        {
            A[i++] = B[j++];
        }
        if ((k < lc) && (!(j < lb) || (C[k] < B[j])))
        {
            A[i++] = C[k++];
        }
    }
    delete[] B;
}
void mergeSort(vector<int> &data, int lo, int hi)
{
    int num = 0;
    if (hi - lo < 2)
        return;
    int mi = (lo + hi) / 2;
    mergeSort(data, lo, mi);
    mergeSort(data, mi, hi);
    merge(data, lo, mi, hi);
}
struct Cnt
{
    int val;
    int cnt;
    Cnt(int val, int cnt) : val(val), cnt(cnt) {}
};
class Queap
{
public:
    Queap() {}
    void push(int x);
    int pop();
    int getMax();
    int countnum() { return que.size(); }

private:
    queue<int> que; //单项队列   用于正常存储数据
    deque<Cnt> cnt; //双向队列     用于动态维护前x天的最大值
};

void Queap::push(int x)
{
    que.push(x);

    int count = 1;
    while (!cnt.empty() && x > cnt.back().val)
    {
        count += cnt.back().cnt;
        cnt.pop_back();
    }
    cnt.push_back(Cnt(x, count));
}

int Queap::pop()
{
    int ret = que.front();
    que.pop();
    if (cnt.front().cnt > 1)
    {
        cnt.front().cnt -= 1;
    }
    else
        cnt.pop_front();

    return ret;
}
int Queap::getMax()
{
    return cnt.front().val;
}
int days;
int outputtime;
vector<int> maxcnt; //用于存放最大值的结果，之后使用二分查找得出对应的p，q的结果

Queap result;        //每天确诊的人数 用队堆实现
vector<int> data;    //先接收数据
vector<int> require; //向前回溯的天数
vector<int> p;       //低要求
vector<int> q;       //中要求
void receivedata()
{
    // cout<<"进入接受数据函数"<<endl;
    cin >> days;
    for (int i = 0; i < days; i++)
    {
        int data1;
        cin >> data1;
        if (i == 0)
            data.push_back(-1); //将最后一个数据挤掉，用-1在前面补充，不会影响结果
        if (i != days - 1)
            data.push_back(data1);
    }
    // cout<<data.size()<<" "<<days<<endl;
    // cout<<"每天确诊数据已输入"<<endl;
    for (int i = 0; i < days; i++)
    {
        int data2;
        cin >> data2;
        require.push_back(data2);
    }

    // cout<<"追溯天数已输入"<<endl;
    cin >> outputtime;
    for (int j = 0; j < outputtime; j++) //接受每次的p q值
    {
        int data1, data2;
        cin >> data1 >> data2;
        p.push_back(data1);
        q.push_back(data2);
    }
    // cout<<"数据已全部接受完成"<<endl;
}
int main()
{
    receivedata();
    for (int i = 0; i < days; i++)
    {
        result.push(data[i]);
        while (result.countnum() > require[i])
        {
            // cout<<"进入pop循环"<<endl;
            result.pop();
        }

        maxcnt.push_back(result.getMax()); //经过处理后，maxcnt的前x天最大确诊数已经确定，接下来是用二分归并排序处理 然后用
                                           // p,q处理结果
    }
    mergeSort(maxcnt, 0, maxcnt.size());
    maxcnt[0] = 0;
    for (int i = 0; i < p.size(); i++)
    {
        int a1, a2;
        a1 = binSearch(maxcnt.data(), p[i], 0, maxcnt.size());
        a2 = binSearch(maxcnt.data(), q[i], 0, maxcnt.size()); //找到不大于p，q的最大下缀
        while (maxcnt[a1] == p[i])
            a1--;
        while (maxcnt[a2] == q[i])
            a2--;
        cout << a1 + 1 << " " << a2 - a1 << endl;
    }
    return 0;
}
