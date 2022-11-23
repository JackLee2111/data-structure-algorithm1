//实现O(n)复杂度的栈混洗算法
// example：输入为3，1，2，4，输出为False

// Stack_Washing algorithm.cpp
// created by LJL on 2022-10-07
//基本思路：借助A，B，S栈来模拟混洗过程

#include <iostream>
#include <string>
#include <stack>
using namespace std;

//设a为输入栈,假设是12345……；b为要验证的栈
/*
先将要验证的栈B转移到栈m，这样B的栈底就位于m的栈顶了
再把a中的push进s中，模拟混洗的过程。如果m栈顶与s中一样，那么就将m和s都pop。
如果s为空，则验证通过栈混洗可以得到栈B。
*/

bool stack_Washing(stack<int> a, stack<int> b)
{
    stack<int> m, s;
    while (b.size())
    {
        m.push(b.top());
        b.pop();
    }
    while (a.size())
    {
        s.push(a.top());
        a.pop();
        //查看底部元素是否可以完成操作
        if (s.top() == m.top())
        {
            s.pop();
            m.pop();
            while (s.size() && (s.top() == m.top()))
            { //将s中累计的元素清算
                s.pop();
                m.pop();
            }
        }
    }
    // s为空,栈清洗成功
    return s.empty();
}
int main()
{
    int n;
    //栈中元素个数
    cin >> n;
    //原本输入的栈，假设为[5,4,3,2,1>的形式
    stack<int> A;
    for (int i = n; i >= 1; i--)
    {
        A.push(i);
    }
    //要验证的栈
    stack<int> B;
    for (int i = 1; i <= n; i++)
    {
        int x;
        cin >> x;
        B.push(x);
    }

    bool flag = stack_Washing(A, B);
    //匹配返回True，否则返回False
    if (flag == false)
        cout << "False" << endl;
    else
        cout << "True" << endl;

    return 0;
}