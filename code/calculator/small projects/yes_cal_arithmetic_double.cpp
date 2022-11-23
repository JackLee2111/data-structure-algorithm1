#include <iostream>
#include <algorithm>
#include <string>
#include <stack>
using namespace std;
//可直接调用，把stack方法写好就行
//用到的方法：stack.push(),stack.pop(),stack.top

//判断是否是字符
bool isdigit(char c)
{
    return (c >= '0' && c <= '9');
}

// string的字符转成double数字（使用std::stod）
double get_op(const string &expression, int &pos)
{
    string operand = "";
    while (expression[pos] == '.' || isdigit(expression[pos]))
        operand.push_back(expression[pos++]);

    double num = stod(operand);
    return num;
}

//后缀表达式求值
double cal_arithmetic(string str)
{
    int len = str.length();
    int pos = 0;
    char c;
    stack<double> num_stack;

    //遍历
    while (pos < len)
    {
        if (isdigit(str[pos]))
        {
            double op = get_op(str, pos);
            num_stack.push(op);
        }
        else if (str[pos] == ' ')
            pos++;
        else
        {
            double num1, num2;
            c = str[pos++];
            num2 = num_stack.top();
            num_stack.pop();
            num1 = num_stack.top();
            num_stack.pop();

            switch (c)
            {
            case '+':
                num_stack.push(num1 + num2);
                break;
            case '-':
                num_stack.push(num1 - num2);
                break;
            case '*':
                num_stack.push(num1 * num2);
                break;
            case '/':
                num_stack.push(num1 / num2);
                break;
            }
        }
    }
    double ans = num_stack.top();
    return ans;
}

//中缀表达式 转 后缀表达式
string Infix_to_postfix(string str)
{
    stack<string> num_stack, operator_stack;
    int len = str.length();
    int pos = 0;

    while (pos < len)
    {
        if (isdigit(str[pos]))
        {
            double op = get_op(str, pos);
            num_stack.push(to_string(op));
        }
        else if (str[pos] == ' ')
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
