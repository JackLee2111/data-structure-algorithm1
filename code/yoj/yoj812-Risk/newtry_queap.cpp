#include <iostream>
#include <algorithm>
#include <math.h>
#include <string>
#include <vector>
#include <queue>
using namespace std;

int main()
{
    int n;
    vector<unsigned int> x;
    vector<unsigned int> m;
    queue<unsigned int> getmax;

    vector<unsigned int> max_case;
    int T;
    unsigned int p, q;

    //输入
    cin >> n;

    x.push_back(0);
    m.push_back(0);
    //病例
    for (int i = 1; i <= n; i++)
    {
        unsigned int xx;
        cin >> xx;
        x.push_back(xx);
    }
    //追溯时间
    for (int i = 1; i <= n; i++)
    {
        unsigned int mm;
        cin >> mm;
        m.push_back(mm);
    }

    //第0天
    max_case.push_back(0);
    //第1天
    max_case.push_back(0);

    //处理，计算每一天的最大值

    //先处理前面每天都要计算的部分
    int head = 0, tail = 0;
    int i = 2;
    while (i - m[i] <= 1)
    {
        while (head <= tail && a[q[tail]] <= a[i])
            tail--;
        q[++tail] = i;
        i++;
    }

    for (int i = 1; i < k; i++)
    {
        while (head <= tail and a[q[tail]] <= a[i])
            tail--;
        q[++tail] = i;
    }
    for (int i = k; i <= n; i++)
    {
        while (head <= tail and a[q[tail]] <= a[i])
            tail--;
        q[++tail] = i;
        while (q[head] <= i - k)
            head++;
        cout << a[q[head]] << '\n';
    }

    while (n-- > 0)
    {
        cin >> p >> q;
        unsigned int low_danger = 0;
        unsigned int mid_danger = 0;
        //第一天之前都为0
        // max_case.push_back(0);
        low_danger++;

        for (int i = 1; i < n; i++)
        {
            //找到前i-days天的最大值
            int days = m[i];
            unsigned int maxx = 0;
            if (i - days < 0)
                days = i;
            for (int j = i - days; j < i; j++)
            {
                if (x[j] > maxx)
                    maxx = x[j];
            }
            // cout << "maxx" << maxx << endl;
            if (maxx >= 0 && maxx < p)
            {
                low_danger++;
                // cout << "low plus i:" << i << endl;
            }

            else if (maxx >= p && maxx < q)
                mid_danger++;
        }

        cout << low_danger << " " << mid_danger << endl;
    }

    cin >> T;

    return 0;
}
