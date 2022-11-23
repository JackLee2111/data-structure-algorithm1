#include <iostream>
#include <cstdio>
//#include <Eigen/Dense>
#include "G:\\2022 autumn semester\\data structure and algorithm\\eigen-3.4.0\\Eigen\\Dense"

using namespace std;
int main()
{
    Eigen::Matrix<double, 4, 4> m1; //  4x4 的 float 类型的矩阵 m1
    // Eigen::MatrixXf m1(4, 4);      //  定义一个 4x4 的 float 类型的动态矩阵
    /*
    m1 << 1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16;      //  赋值
    std::cout << m1 << endl; //  输出 m
    */
    //输入矩阵
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            double x;
            m1 << x;
        }
    }

    Eigen::Matrix<double, 4, 4> m2;
    m2 << 1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16;      //  赋值
    std::cout << m2 << endl; //  输出 m

    //加减
    Eigen::Matrix<double, 4, 4> m3;
    Eigen::Matrix<double, 4, 4> m4;
    m3 = m1 + m2;
    m4 = m1 - m2;
    cout << m3 << endl;
    cout << m4 << endl;

    //乘法
    Eigen::Matrix<double, 4, 4> m5;
    m5 = m1 * m2;
    cout << m5 << endl;

    //转置
    Eigen::Matrix<double, 4, 4> a;
    a = m1.transpose(); //  获取 m 的转置矩阵
    cout << a << endl;

    cout << "tezhengzhi" << a.eigenvalues() << endl; //矩阵的特征值

    double ans2 = a.determinant(); //矩阵求行列式的值
    cout << "determinant:" << ans2 << endl;

    return 0;
}