#include <stdio.h>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
using namespace std;
struct Cnt
{
    int val;
    int cnt;
    Cnt(int val, int cnt) : val(val), cnt(cnt) {}
};

// Queap类
class Queap
{
public:
    Queap() {}
    void push(int x);
    int pop();
    int getMax();
    int size_of_queue() { return que.size(); }

private:
    queue<int> que;   // 单项队列，正常存储数据
    deque<Cnt> queap; // 双向队列，动态维护前x天的最大值
};

void Queap::push(int x)
{
    que.push(x); // 单项队列，正常存储

    int count = 1;                                 // 数值，有几个
    while (!queap.empty() && x > queap.back().val) // 双向队列
    {
        count += queap.back().cnt;
        queap.pop_back();
    }
    queap.push_back(Cnt(x, count));
}

int Queap::pop()
{
    int r = que.front();
    que.pop();
    if (queap.front().cnt > 1)
    {
        queap.front().cnt -= 1;
    }
    else
        queap.pop_front();

    return r;
}
int Queap::getMax()
{
    return queap.front().val;
}

vector<int> max_ans; // 用于存放最大值的结果，之后使用二分查找得出对应的p，q的结果

Queap result;     // 每天确诊的人数 Queap
vector<int> data; // 输入的确诊数
vector<int> ask;  // 回溯的天数
vector<int> p;    // 低风险
vector<int> q;    // 中风险
int days;         // 天数
void get_input()
{
    cin >> days;
    // 每天确诊数据
    for (int i = 0; i < days; i++)
    {
        int cases;
        cin >> cases;
        if (i == 0)
            data.push_back(-1); // 第一天没意义，-1补充
        if (i != days - 1)
            data.push_back(cases); // 最后一天的数据没必要，用-1在前面补充，不会影响结果
    }

    // 追溯天数
    for (int i = 0; i < days; i++)
    {
        int back_search_days;
        cin >> back_search_days;
        ask.push_back(back_search_days);
    }

    int case_time;
    cin >> case_time;
    for (int j = 0; j < case_time; j++) // 接受每次的p q值
    {
        int data_p, data_q;
        cin >> data_p >> data_q;
        p.push_back(data_p);
        q.push_back(data_q);
    }
}

// 二分查找
int binSearch(int *A, int const &e, int l, int h)
{
    while (l < h)
    {
        int m = (l + h) >> 1;
        (e < A[m]) ? h = m : l = m + 1;
    }

    return --l; // 循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
} // 有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置

int main()
{
    get_input();
    for (int i = 0; i < days; i++)
    {
        result.push(data[i]);
        while (result.size_of_queue() > ask[i]) // queap中的元素多于向前询问的天数，就可以从队头pop了，因为再也用不到
        {
            result.pop();
        }
        max_ans.push_back(result.getMax()); // max_ans的前x天维护之后queap中最大确诊数，然后用二分归并排序处理，用p,q处理结果
    }
    sort(max_ans.begin(), max_ans.end());
    max_ans[0] = 0; // 处理第一天

    int len = p.size();
    for (int i = 0; i < p.size(); i++)
    {
        int ans1, ans2;
        // 找到不大于p，q的最大值
        ans1 = binSearch(max_ans.data(), p[i], 0, max_ans.size()); // ans1为等于p或者小于p的最后一天
        ans2 = binSearch(max_ans.data(), q[i], 0, max_ans.size()); // ans2为等于q或者小于q的最后一天

        // 如果ans1为等于p，还要往前找小于p的最后一天
        while (max_ans[ans1] == p[i])
            ans1--;
        // 同上
        while (max_ans[ans2] == q[i])
            ans2--;
        cout << ans1 + 1 << " ";
        cout << ans2 - ans1 << endl;
    }
    return 0;
}
