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
#define INPUT_POJ "./data/poj.txt" // ASCII
#define OUTPUT_POJ "./data/test_a2.in"
// #define OUTPUT_POJ "./data/1-1.in"
#define INPUT_HDU "./data/hdu.txt" // utf-8
#define OUTPUT_HDU "./data/test_u1.in"
// 数据特征参数
#define DATA_NUM 10000                      // 插入组数
#define ASK_NUM 9000                        // 查询组数
#define ASK_NOT_IN_NUM 1000                 // 不在插入中的组数
#define ASK_IN_NUM ASK_NUM - ASK_NOT_IN_NUM // 在插入中的组数
/*三组数据4个参数分别为
 *1.
 *2.
 *3.
 */

#define MOD 233331
string out_name[MAX_NUM];
string prepare_name[MAX_NUM];
int out_number[MAX_NUM];
int line1[MAX_NUM]; // 随机取行

bool cmp(int a, int b)
{
    return a > b;
}
void creat_data_for_poj()
{
    int data_num1 = DATA_NUM;             // 数据组数
    int ask_num1 = ASK_NUM;               // 询问组数
    int ask_not_in_num1 = ASK_NOT_IN_NUM; // 不存在的询问组数
    int ask_in_num1 = ASK_IN_NUM;         // 存在的询问组数

    srand((unsigned)time(NULL));
    for (int i = 1; i <= 3 * data_num1; i++)
    {
        line1[i] = rand() % MOD;
    }

    sort(line1 + 1, line1 + 3 * data_num1 + 1, cmp);
    unique(line1 + 1, line1 + 3 * data_num1 + 1);
    /*
    for (int i = 1; i <= 2 * data_num1; i++)
    {
        cout << line1[i] << endl;
    }*/

    ifstream infile1;  // 输入流
    ofstream outfile1; // 输出流
    infile1.open(INPUT_POJ, ios::in);
    if (!infile1.is_open())
        cout << "Open in-file failure!" << endl;
    int i1 = 0, i_line1 = 1;
    while (!infile1.eof()) // 若未到文件结束一直循环
    {
        i1++;
        int number, rank, pro;
        string name;
        infile1 >> number >> name >> rank >> pro;

        if (i_line1 <= data_num1 && line1[i_line1] == i1)
        {
            out_name[i_line1] = name;
            out_number[i_line1] = i1;
            i_line1++;
        }
        else if (i_line1 > data_num1 && i_line1 <= 2 * data_num1 && line1[i_line1] == i1)
        {
            prepare_name[i_line1 - data_num1] = name;
            i_line1++;
        }
        else if (i_line1 > 2 * data_num1)
            break;
    }

    outfile1.open(OUTPUT_POJ, ios::app);
    if (!outfile1.is_open())
        cout << "Open out-file failure!" << endl;

    // cout << "--------" << endl;
    for (int i = 1; i <= ask_in_num1; i++)
    {
        prepare_name[i] = out_name[i];
        // cout << out_name[i] << endl;
    }
    // cout << "--------" << endl;

    for (int k = 1, i = 1, j = 1; k <= data_num1 + ask_num1; k++)
    {
        if (k < (data_num1 + ask_num1) / 3)
        {
            outfile1 << "0 " << out_name[i] << " " << out_number[i] << endl;
            // cout << "0 " << out_name[i] << " " << out_number[i] << endl;
            i++;
            continue;
        }
        if (line1[k] % 2 == 1 && i <= data_num1)
        {
            outfile1 << "0 " << out_name[i] << " " << out_number[i] << endl;
            // cout << "0 " << out_name[i] << " " << out_number[i] << endl;
            i++;
        }
        else if (line1[k] % 2 == 1 && i > data_num1)
        {
            outfile1 << "1 " << prepare_name[j] << endl;
            // cout << "1 " << prepare_name[j] << endl;
            j++;
        }
        else if (line1[k] % 2 == 0 && j <= ask_num1)
        {
            outfile1 << "1 " << prepare_name[j] << endl;
            // cout << "1 " << prepare_name[j] << endl;
            j++;
        }
        else if (line1[k] % 2 == 0 && j > ask_num1)
        {
            outfile1 << "0 " << out_name[i] << " " << out_number[i] << endl;
            // cout << "0 " << out_name[i] << " " << out_number[i] << endl;
            i++;
        }
    }

    outfile1 << "2" << endl;
    cout << "end1" << endl;
    /*
    for (int i = 1; i <= data_num1; i++)
    {
        outfile1 << "0 " << out_name[i] << " " << out_number[i] << endl;
    }
    for (int i = 1; i <= data_num1; i++)
    {
        outfile1 << "1 " << prepare_name[i] << endl;
    }*/

    infile1.close();
    outfile1.close();
}

int line2[MAX_NUM]; // 随机取行
void creat_data_for_hdu()
{
    int data_num2 = DATA_NUM;             // 数据组数
    int ask_num2 = ASK_NUM;               // 询问组数
    int ask_not_in_num2 = ASK_NOT_IN_NUM; // 不存在的询问组数
    int ask_in_num2 = ASK_IN_NUM;         // 存在的询问组数

    srand((unsigned)time(NULL));
    for (int i = 1; i <= 3 * data_num2; i++)
    {
        line2[i] = rand() % MOD;
    }

    sort(line2 + 1, line2 + 3 * data_num2 + 1, cmp);
    unique(line2 + 1, line2 + 3 * data_num2 + 1);
    /*
    for (int i = 1; i <= 2 * data_num1; i++)
    {
        cout << line1[i] << endl;
    }*/
    ifstream infile2;  // 输入流
    ofstream outfile2; // 输出流
    infile2.open(INPUT_HDU, ios::in);
    if (!infile2.is_open())
        cout << "Open in-file failure!" << endl;
    int i2 = 500000, i_line2 = 1;
    while (!infile2.eof()) // 若未到文件结束一直循环
    {
        i2--;
        int number, rank, pro;
        string name;
        infile2 >> number >> name >> rank >> pro;

        if (i_line2 <= data_num2 && line2[i_line2] == i2)
        {
            out_name[i_line2] = name;
            out_number[i_line2] = i2;
            i_line2++;
        }
        if (i_line2 > data_num2 && i_line2 <= 2 * data_num2 && line2[i_line2] == i2)
        {
            prepare_name[i_line2 - data_num2] = name;
            i_line2++;
        }
        if (i_line2 > 2 * data_num2)
            break;
    }
    outfile2.open(OUTPUT_HDU, ios::app);
    if (!outfile2.is_open())
        cout << "Open out-file failure!" << endl;
    // cout << "--------" << endl;
    for (int i = 1; i <= ask_in_num2; i++)
    {
        prepare_name[i] = out_name[i];
        // cout << out_name[i] << endl;
    }
    // cout << "--------" << endl;
    for (int k = 1, i = 1, j = 1; k <= data_num2 + ask_num2; k++)
    {
        if (k < (data_num2 + ask_num2) / 3)
        {
            outfile2 << "0 " << out_name[i] << " " << out_number[i] << endl;
            i++;
            continue;
        }
        if (line2[k] % 2 == 1 && i <= data_num2)
        {
            outfile2 << "0 " << out_name[i] << " " << out_number[i] << endl;
            i++;
        }
        else if (line2[k] % 2 == 1 && i > data_num2)
        {
            outfile2 << "1 " << prepare_name[j] << endl;
            j++;
        }
        else if (line2[k] % 2 == 0 && j <= ask_num2)
        {
            outfile2 << "1 " << prepare_name[j] << endl;
            j++;
        }
        else if (line2[k] % 2 == 0 && j > ask_num2)
        {
            outfile2 << "0 " << out_name[i] << " " << out_number[i] << endl;
            i++;
        }
    }
    outfile2 << "2" << endl;
    cout << "end2" << endl;

    infile2.close();
    outfile2.close();
}

int main()
{
    // poj.txt文件操作
    // creat_data_for_poj();

    // hdu.txt文件操作
    creat_data_for_hdu();

    return 0;
}