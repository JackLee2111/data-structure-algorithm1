#include <iostream>
#include <string>
#include <algorithm>
#include <fstream> //文件流库函数
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;
#define MAX_NUM 130000

// 输入输出文件
#define INPUT_POJ "./data/temp2.txt" // ASCII
#define OUTPUT_POJ "./data/temp3.txt"

// 数据特征参数
#define DATA_NUM 300000 // 插入组数

void delete1()
{
    int data_num1 = DATA_NUM; // 数据组数

    ifstream infile;  // 输入流
    ofstream outfile; // 输出流
    infile.open(INPUT_POJ, ios::in);
    if (!infile.is_open())
        cout << "Open in-file failure!" << endl;
    outfile.open(OUTPUT_POJ, ios::app);
    if (!outfile.is_open())
        cout << "Open out-file failure!" << endl;

    int i = 0;
    string temp;
    while (getline(infile, temp)) // 按行读取字符串
    {
        i++;
        /*
        if (i <= DATA_NUM)
            continue;*/
        outfile << temp; // 写文件
        outfile << endl;
        if (i == DATA_NUM)
            break;
    }

    infile.close();
    outfile.close();
}

int main()
{
    // poj.txt文件操作
    delete1();

    // hdu.txt文件操作
    // creat_data_for_hdu();

    return 0;
}