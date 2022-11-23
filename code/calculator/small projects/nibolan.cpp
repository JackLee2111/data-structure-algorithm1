#include <iostream>
#include <algorithm>
#include <string>
#include <stack>
using namespace std;
string str;

long long calculate(string str)
{
    int len = str.length(), pos = 0;
    stack<long long> sta;
    char ch;

    while (pos < len)
    {
        if (str[pos] >= '0' && str[pos] <= '9')
        {
            long long temp = 0;
            while (str[pos] != ' ' && str[pos] != '.')
                temp = temp * 10 + (str[pos++] - '0');
            sta.push(temp);
        }
        else if (str[pos] == ' ' || str[pos] == '.')
            pos++;
        else if (str[pos] == '@')
            break;
        else
        {
            ch = str[pos++];
            int num1, num2;
            num2 = sta.top();
            sta.pop();
            num1 = sta.top();
            sta.pop();

            switch (ch)
            {
            case '+':
                sta.push(num1 + num2);
                break;
            case '-':
                sta.push(num1 - num2);
                break;
            case '*':
                sta.push(num1 * num2);
                break;
            case '/':
                sta.push(num1 / num2);
                break;
            }
        }
    }
    return sta.top();
}

int main()
{
    getline(cin, str);
    cout << calculate(str);
    return 0;
}