#include <iostream>
#include <stack>
using namespace std;

int main()
{
    int n;
    //元素个数
    cin >> n;

    int *data = new int[n];
    for (int i = 0; i < n; i++)
        cin >> data[i];
    stack<int> s;

    for (int i = 1; i < data[0]; i++)
        s.push(i);

    int tmp = data[0];
    for (int i = 1; i < n; i++)
    {
        if (!s.empty())
        {
            if (data[i] == s.top())
                s.pop();                //当前元素与栈顶元素相同时，取出栈顶元素
            else if (data[i] > s.top()) //当前元素大于栈顶元素，则让后续数字入栈
                for (int j = tmp + 1; j < data[i]; j++)
                    s.push(j);
            else if (data[i] < s.top()) //当前元素小于栈顶元素，则不可能有这样的出栈顺序
            {
                cout << "false" << endl;
                return 0;
            }
        }
    }
    cout << "true" << endl;
    return 0;
}
