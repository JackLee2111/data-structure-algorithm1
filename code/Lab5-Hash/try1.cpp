#include "stdafx.h"
#include <sstream>
#include <string>

using namespace std;

ifstream &seek_to_line(ifstream &in, int line);

int main()
{
    string labels_txt = "E:/bdd100k.txt";
    int line = 100; // 按逻辑修改行数
    ifstream file;
    file.open(labels_txt);
    if (!file && flag)
        cerr << "数据集标注txt文件不存在,检查路径！" << endl;
    seek_to_line(file, line);   // 定位到txt的指定行
    while (getline(file, data)) // 按行读取txt
    {
        // 操作
    }
    file.close();
}

// 定位到txt文件的某一行
ifstream &seek_to_line(ifstream &in, int line)
// 将打开的文件in，定位到line行。
{
    int i;
    char buf[1024];
    in.seekg(0, ios::beg); // 定位到文件开始。
    for (i = 0; i < line; i++)
    {
        in.getline(buf, sizeof(buf)); // 读取行。
    }
    return in;
}