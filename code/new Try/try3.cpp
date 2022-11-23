#include <iostream>
#include <algorithm>
#include <math.h>
#include <string>
#include <vector>
using namespace std;

//一元多项式操作——————————————————————————————————————————————————————————————————
//多项式输出——————
void output_poly(vector<double> poly_ans, int times)
{
    int flag = 0;
    if (poly_ans[0] != 0)
    {
        cout << poly_ans[0];
        flag = 1;
    }
    for (int i = 1; i <= times; i++)
    {
        if (poly_ans[i] == 0)
            continue;

        else if (poly_ans[i] > 0)
        {
            if (i == 1 && flag == 0)
            {
                if (poly_ans[i] == 1)
                    cout << "x";
                else
                    cout << poly_ans[i] << "x";
                flag = 1;
            }
            else if (i == 1 && flag == 1)
            {
                if (poly_ans[i] == 1)
                    cout << "+x";
                else
                    cout << "+" << poly_ans[i] << "x";
            }
            else if (i != 1 && flag == 0)
            {
                if (poly_ans[i] == 1)
                    cout << "x^" << i;
                else
                    cout << poly_ans[i] << "x^" << i;
                flag = 1;
            }
            else if (i != 1 && flag == 1)
            {
                if (poly_ans[i] == 1)
                    cout << "+x^" << i;
                else
                    cout << "+" << poly_ans[i] << "x^" << i;
            }
        }
        else if (poly_ans[i] < 0)
        {
            if (i == 1)
            {
                if (poly_ans[i] == -1)
                    cout << "-x";
                else
                    cout << poly_ans[i] << "x";
            }
            else
            {
                if (poly_ans[i] == -1)
                    cout << "-x^" << i;
                else
                    cout << poly_ans[i] << "x^" << i;
            }
        }
    }
    if (flag == 0)
        cout << '0';
    cout << endl;
}

//多项式加法
void poly_plus(vector<double> &poly1_item, vector<double> &poly2_item, int t1, int t2)
{
    int times = max(t1, t2);
    vector<double> poly_ans;

    //加法运算
    for (int i = 0; i <= times; i++)
    {
        poly_ans.push_back(poly1_item[i] + poly2_item[i]);
    }
    //多项式输出
    output_poly(poly_ans, times);
    return;
}

//多项式减法
void poly_minus(vector<double> &poly1_item, vector<double> &poly2_item, int t1, int t2)
{
    int times = max(t1, t2);
    vector<double> poly_ans;
    //减法运算
    for (int i = 0; i <= times; i++)
    {
        poly_ans.push_back(poly1_item[i] - poly2_item[i]);
    }
    //多项式输出
    output_poly(poly_ans, times);
    return;
}

//多项式乘法
void poly_multi(vector<double> &poly1_item, vector<double> &poly2_item, int t1, int t2)
{
    int times = t1 + t2;
    vector<double> poly_ans;
    double multi[30][30] = {0};
    double ans[100] = {0};
    //乘法运算
    for (int i = 0; i <= t1; i++)
    {
        for (int j = 0; j <= t2; j++)
        {
            multi[i][j] = poly1_item[i] * poly2_item[j];
        }
    }
    ans[0] = multi[0][0];
    for (int k = 1; k <= times; k++)
    {
        for (int i = 0, j = k; j >= 0; i++, j--)
        {
            ans[k] += multi[i][j];
        }
    }
    for (int k = 0; k <= times; k++)
    {
        poly_ans.push_back(ans[k]);
    }

    //多项式输出
    output_poly(poly_ans, times);
    return;
}
//一元多项式操作——————————————————————————————————————————————————————————————————
int main()
{
    string poly1;
    string poly2;
    int t1, t2, op;
    cout << "Please enter the highest time of the first polynomial:" << endl;
    cin >> t1;
    cout << "please input the coefficient of the first polynomial:" << endl;
    cout << "(for example:4 3 4 0 5 represent 4+3x+4x^2+5x^4)" << endl;
    vector<double> poly1_item;
    for (int i = 1; i <= t1 + 1; i++)
    {
        double x;
        cin >> x;
        poly1_item.push_back(x);
    }

    cout << "Please enter the highest time of the second polynomial:" << endl;
    cin >> t2;
    cout << "please input the coefficient of the second polynomial:" << endl;
    vector<double> poly2_item;
    for (int i = 0; i <= t2; i++)
    {
        double x;
        cin >> x;
        poly2_item.push_back(x);
    }

    cout << "please input the the operation(1:+  2:-  3:*):" << endl;
    cin >> op;

    //+
    if (op == 1)
    {
        poly_plus(poly1_item, poly2_item, t1, t2);
    }

    //-
    else if (op == 2)
    {
        poly_minus(poly1_item, poly2_item, t1, t2);
    }

    //*
    else if (op == 3)
    {
        poly_multi(poly1_item, poly2_item, t1, t2);
    }

    //非法输入
    else
        cout << "Illegal operation!" << endl;
}