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
    vector<unsigned int> max_case;
    int T;
    unsigned int p, q;

    //输入
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        unsigned int xx;
        cin >> xx;
        x.push_back(xx);
    }
    for (int i = 0; i < n; i++)
    {
        unsigned int mm;
        cin >> mm;
        m.push_back(mm);
    }
    cin >> T;

    //处理
    while (T-- > 0)
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
    return 0;
}
