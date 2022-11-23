#include <algorithm>
#include <queue>

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

private:
    queue<int> que;
    deque<Cnt> cnt;
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

class Solution
{
public:
    vector<int> maxSlidingWindow(vector<int> &nums, int k)
    {
        Queap que;
        vector<int> ret;
        if (nums.size() == 0)
            return vector<int>();

        int i = 0;
        for (; i < k; i++)
            que.push(nums[i]);
        ret.push_back(que.getMax());

        for (; i < nums.size(); i++)
        {
            que.pop();
            que.push(nums[i]);
            ret.push_back(que.getMax());
        }
        return ret;
    }
};