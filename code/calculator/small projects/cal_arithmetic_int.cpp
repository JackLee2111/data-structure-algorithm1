#include <iostream>
#include <algorithm>
#include <string>
#include <stack>
using namespace std;

//后缀表达式求值(ver int)
long long cal_arithmetic(string str)
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

//中缀表达式转后缀表达式
string Infix_to_postfix(string str)
{
    stack<string> num_stack, operator_stack;
    int len = str.length();
    int pos = 0;

    while (pos < len)
    {
        if (str[pos] >= '0' && str[pos] <= '9')
        {
            long long temp = 0;
            while (str[pos] >= '0' && str[pos] <= '9')
                temp = temp * 10 + (str[pos++] - '0');
            num_stack.push(to_string(temp));
        }
        else if (str[pos] == ' ' || str[pos] == '.')
            pos++;
        else
        {
            char ch = str[pos++];
            if (ch == '+')
            {
                while (!operator_stack.empty() && (operator_stack.top() == "*" || operator_stack.top() == "/"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                while (!operator_stack.empty() && (operator_stack.top() == "+" || operator_stack.top() == "-"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                operator_stack.push("+");
            }
            else if (ch == '-')
            {
                while (!operator_stack.empty() && (operator_stack.top() == "*" || operator_stack.top() == "/"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                while (!operator_stack.empty() && (operator_stack.top() == "+" || operator_stack.top() == "-"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                operator_stack.push("-");
            }
            else if (ch == '*')
            {
                while (!operator_stack.empty() && (operator_stack.top() == "*" || operator_stack.top() == "/"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                operator_stack.push("*");
            }
            else if (ch == '/')
            {
                while (!operator_stack.empty() && (operator_stack.top() == "*" || operator_stack.top() == "/"))
                    num_stack.push(operator_stack.top()), operator_stack.pop();
                operator_stack.push("/");
            }
            else if (ch == '(')
                operator_stack.push("(");
            else
            {
                while (operator_stack.top() != "(")
                {
                    num_stack.push(operator_stack.top());
                    operator_stack.pop();
                }
                operator_stack.pop();
            }
        }
    }
    while (!operator_stack.empty())
    {
        num_stack.push(operator_stack.top());
        operator_stack.pop();
    }

    string ans = "";
    while (!num_stack.empty())
        ans = num_stack.top() + " " + ans, num_stack.pop();
    return ans;
}

int main()
{
    string str;
    getline(cin, str);
    cout << cal_arithmetic(Infix_to_postfix(str));
    return 0;
}
