#include <stack>
using namespace std;

template <typename T>
bool stackWashing(stack<T> A, stack<T> B)
{
    //模拟栈混洗的过程
    stack<T> S, T;

    //将B中的元素还原进S中
    while (B.size())
    {
        S.push(B.top());
        B.pop();
    }

    //将A中的元素还原进T中
    while (A.size())
    {
        T.push(A.top());
        A.pop();
    }

    while (T.size())
    {
        if (S.size() && S.top() == T.top())
        {
            S.pop();
            T.pop();
        }
        else
        {
            return false;
        }
    }
    return S.size();
}
