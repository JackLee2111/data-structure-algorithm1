#include <iostream>
#include <algorithm>
#include <string>
#include <stack>
using namespace std;

string TRANS_TO_POLISH(string str)
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
    cout << TRANS_TO_POLISH(str);
    return 0;
}