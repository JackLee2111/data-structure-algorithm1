//对括号的嵌套匹配检查
// parenthesis matching.cpp
// created by LJL on 2022-10-07

//基本思路：遇到左括号入栈；遇到有括号比对相应的左括号，出栈

#include <iostream>
#include <algorithm>
#include <math.h>
#include <string>
#include <vector>
#include <stack>
using namespace std;

//表达式括号匹配检查,可检查三类括号[{(
bool check_paren(string str)
{
    stack<char> S;
    char x, y, z;
    for (int i = 0; i <= str.size(); i++)
    {
        char c = str[i];
        //遇到左括号，入栈
        switch (c)
        {
        case '(':
        case '[':
        case '{':
            S.push(c);
            break;
        //遇到右括号，判断是否为空或者能否与相应的左括号对应，否则返回false
        case ')':
        {
            if (S.empty())
                return false;
            x = S.top();
            S.pop();
            if ('(' != x)
                return false;
            break;
        }
        case ']':
        {
            if (S.empty())
                return false;
            y = S.top();
            S.pop();
            if ('[' != y)
                return false;
            break;
        }
        case '}':
        {
            if (S.empty())
                return false;
            z = S.top();
            S.pop();
            if ('{' != z)
                return false;
            break;
        }

        default:
            break;
        }
    }
    //如果栈空，则括号恰好匹配；否则不匹配
    return S.empty();
}
int main()
{
    string str;
    getline(cin, str);
    // int len = str.length();
    bool flag;

    flag = check_paren(str);
    //匹配返回True，否则返回False
    if (flag == false)
        cout << "False" << endl;
    else
        cout << "True" << endl;
    return 0;
}