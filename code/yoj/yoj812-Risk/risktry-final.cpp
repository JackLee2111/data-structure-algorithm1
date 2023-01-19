#include <stdio.h>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
using namespace std;
struct Data
{
    int value, count;
    Data(int val, int cnt) : value(val), count(cnt) {}
};

// Queap类
class Queap
{
public:
    Queap() {}
    void push(int x);
    int pop();
    int getMax();
    int size_of_queue()
    {
        return que.size();
    }
    queue<int> que;    // 单项队列，正常存储数据
    deque<Data> queap; // 双向队列，动态维护前x天的最大值
};

void Queap::push(int x)
{
    que.push(x); // 单项队列，正常存储

    int count = 1;                                   // 数值，有几个
    while (!queap.empty() && x > queap.back().value) // 双向队列
    {
        count += queap.back().count;
        queap.pop_back();
    }
    queap.push_back(Data(x, count));
}
int Queap::getMax()
{
    return queap.front().value;
}
int Queap::pop()
{
    int r = que.front();
    que.pop();
    if (queap.front().count > 1)
    {
        queap.front().count -= 1;
    }
    else
        queap.pop_front();

    return r;
}
// 二分查找
int binSearch(int *A, int const &x, int l, int h)
{
    while (l < h)
    {
        int m = (l + h) / 2;
        (x >= A[m]) ? l = m + 1 : h = m;
    }
    return --l;
}

int day;             // 天数
Queap result;        // 每天确诊人数 Queap
vector<int> data;    // 输入的确诊数
vector<int> ask;     // 回溯的天数
vector<int> p;       // 低风险
vector<int> q;       // 中风险
vector<int> max_ans; // 存放最大值的结果

int main()
{
    // 数据输入部分
    cin >> day;
    // 每天确诊数据
    for (int i = 0; i < day; i++)
    {
        int cases;
        cin >> cases;
        if (i == 0)
            data.push_back(-1); // 第一天没意义，-1补充
        if (i != day - 1)
            data.push_back(cases); // 最后一天的数据没必要，用-1在前面补充，不会影响结果
    }

    // 追溯天数
    for (int i = 0; i < day; i++)
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
    // 数据读入结束

    for (int i = 0; i < day; i++)
    {
        result.push(data[i]);
        while (result.size_of_queue() > ask[i])
        {
            result.pop();
        }
        max_ans.push_back(result.getMax());
    }
    sort(max_ans.begin(), max_ans.end());
    max_ans[0] = 0;

    int len = p.size();
    for (int i = 0; i < p.size(); i++)
    {
        // 找到不大于p，q的最大值
        int ans1 = binSearch(max_ans.data(), p[i], 0, max_ans.size()); // ans1为等于p或者小于p的最后一天
        int ans2 = binSearch(max_ans.data(), q[i], 0, max_ans.size()); // ans2为等于q或者小于q的最后一天

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
