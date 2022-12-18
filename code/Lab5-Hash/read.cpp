#include <iostream>
#include <fstream> //文件流库函数
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <stdio.h>
#include <ctime>
#define M 1152071 // 桶的容量
using namespace std;
// 独立链节点
struct hashNode1
{
    int key;
    int value;
    struct hashNode1 *next = NULL;
};
struct hashNode1 *Hash_ID1[M + 5];

// 闭散列节点
struct hashNode2
{
    int key;
    int value;
};
struct hashNode2 Hash_ID2[M + 5];
// bool lazyflag[1252009] = {false}; //懒惰删除

//--------冲突处理策略1:独立链法-----------
// 冲突处理策略1:独立链法（插入）
void insert_model1(int id, int v)
{
    int hashCode = abs(id % M);
    struct hashNode1 *now = Hash_ID1[hashCode];
    struct hashNode1 *pre = 0;
    while (now) // 找到最末尾的节点
    {
        pre = now;
        now = now->next;
    }

    now = new hashNode1;
    if (pre) // 待插入节点冲突
    {
        now->key = id;
        now->value = v;
        pre->next = now; // 将now接到pre后面，形成查找链
    }
    else // 要插入的节点未冲突
    {
        now->key = id;
        now->value = v;
        Hash_ID1[hashCode] = now;
    }
}
// 冲突处理策略1:独立链法（查找）
int find_model1(int id)
{
    int hashCode = abs(id % M);
    // cout << "hashCode: " << hashCode << endl;
    struct hashNode1 *now = Hash_ID1[hashCode];
    while (now)
    {
        if (now->key == id)
        {
            // cout << "hash_id: " << now->value << endl;
            return now->value;
        }
        else
            now = now->next;
    }
    return -1;
}

//--------冲突处理策略2:线性试探法-----------
int probe4free2(int key)
{
    int r = abs(key % M);
    while (Hash_ID2[r].key != 0) // 已经有存在值，跳过，继续找空桶
        r = (r + 1) % M;
    return r;
}
int probe4hit2(int key)
{
    int r = abs(key % M);
    // 带删除算法
    // while ((Hash_ID2[r].key != 0 && (key != Hash_ID2[r].key) || (Hash_ID2[r].key==0 && lazyflag[r]==true)))
    while (Hash_ID2[r].key != 0 && (key != Hash_ID2[r].key))
        r = (r + 1) % M;
    return r;
}
// 冲突处理策略2:线性试探法（插入）
void insert_model2(int key, int val)
{
    if (Hash_ID2[probe4hit2(key)].key == key) // 已经存在，不必再插入
        return;
    int r = probe4free2(key);
    Hash_ID2[r].key = key;
    Hash_ID2[r].value = val;
    return;
}
// 冲突处理策略2:线性试探法（查找）
int find_model2(int key)
{
    int r = probe4hit2(key);
    if (Hash_ID2[r].value == 0)
        return -1;
    return Hash_ID2[r].value;
}

//--------冲突处理策略3:双向平方试探法-----------
int probe4free3(int key)
{
    int r = abs(key % M);
    // 双向平方探测
    int i = 1, times = 1, temp_r = r;
    while (Hash_ID2[r].key != 0) // 已经有存在值，跳过，继续找空桶
    {
        if (times % 2 == 1) // 奇数次，加
        {
            r = temp_r;
            r = (r + i * i) % M;
            i++;
            times++;
        }
        else if (times % 2 == 0) // 偶数次，减
        {
            r = temp_r;
            r = (r - i * i) % M;
            i++;
            times++;
        }
    }
    return r;
}
int probe4hit3(int key)
{
    int r = abs(key % M);
    // 带删除算法
    // while ((Hash_ID2[r].key != 0 && (key != Hash_ID2[r].key) || (Hash_ID2[r].key==0 && lazyflag[r]==true)))
    // 双向平方探测
    int i = 1, times = 1, temp_r = r;
    while (Hash_ID2[r].key != 0 && (key != Hash_ID2[r].key))
    {
        if (times % 2 == 1) // 奇数次，加
        {
            r = temp_r;
            r = (r + i * i) % M;
            i++;
            times++;
        }
        else if (times % 2 == 0) // 偶数次，减
        {
            r = temp_r;
            r = (r - i * i) % M;
            i++;
            times++;
        }
    }
    return r;
}
// 冲突处理策略3:双向平方试探法（插入）
void insert_model3(int key, int val)
{
    if (Hash_ID2[probe4hit3(key)].key == key) // 已经存在，不必再插入
        return;
    int r = probe4free3(key);
    Hash_ID2[r].key = key;
    Hash_ID2[r].value = val;
    return;
}
// 冲突处理策略3:双向平方试探法（查找）
int find_model3(int key)
{
    int r = probe4hit3(key);
    if (Hash_ID2[r].value == 0)
        return -1;
    return Hash_ID2[r].value;
}

// utf-8预处理
// 判断utf-8编码下有多少个字节
int get_byte(unsigned char c)
{
    // 通过utf-8字节码进行判断
    int ans = 0;
    if (c >> 7 == 0)
        ans = 1;
    else if (c >> 5 == 0x6)
        ans = 2;
    else if (c >> 4 == 0xE)
        ans = 3;
    else if (c >> 4 == 0xF)
        ans = 4;
    return ans;
}
// 将每个utf-8字符转化成int
int byte_to_int(char *s, int byte)
{
    // cout << "byte_to_int: " << byte << endl;
    int ans = 0;
    int num = 1;
    for (int i = byte - 1; i >= 0; i--)
    {
        ans += int(s[i]) * num;
        num *= 257;
    }
    return ans;
}

// 针对utf-8的哈希函数：将utf-8字符串转成int
int hash_utf8(string str)
{
    int num = 0;
    int code_num[30];
    for (int i = 0; i < str.size();)
    {
        int byte = get_byte(str[i]);
        char c[4][4];

        strncpy(&c[byte][0], &str[i], byte);
        // cout << c[byte] << " ";
        //  cout << "byte: " << byte << endl;
        code_num[num] = byte_to_int(c[byte], byte);
        num++;
        // cout << "num: " << num << endl;
        i += byte;
    }

    int hash = 0;
    for (int i = 0; i < num; i++)
    {
        // cout << code_num[i] << endl;
        hash = (hash << 5) | (hash >> 27);
        hash += code_num[i];
    }
    hash = hash & 0x7FFFFFFF;
    return hash;
}

// ASCII字符串 hashCode(string->int)
int hash_ASCII(string str)
{
    int hash = 0;
    for (int i = 0; i < str.length(); i++)
    {
        hash = (hash << 5) | (hash >> 27);
        hash += (int)str[i];
    }
    hash = hash & 0x7FFFFFFF;
    return hash;
}

int main()
{
    // 读1.in文件操作 ——————————————————
    int opt, num;
    int exist_pos;
    string name;
    ifstream infile;  // 输入流
    ofstream outfile; // 输出流

    time_t begin, end;
    double ret;
    begin = clock();
    int flag = 0;
    // ASCII测试入口
    /*
    infile.open("./data/test_a1.in", ios::in); // final-data/1-1.in  test_a1.in
    outfile.open("./data/test_a1.out", ios::in);
    if (!infile.is_open())
        cout << "Open file failure!" << endl;
    if (!outfile.is_open())
        cout << "Open file failure!" << endl;



    while (!infile.eof()) // 若未到文件结束一直循环
    {
        flag++;
        infile >> opt;
        if (opt == 0)
        {
            infile >> name >> num;
            int i_name = hash_ASCII(name);
            // cout << opt << " " << name << " " << num << " " << i_name << endl;
            //     cout << i_name << endl;
            //       exist_pos = find(i_name);
            insert_model1(i_name, num);
        }
        else if (opt == 1)
        {
            infile >> name;
            // cout << opt << " " << name << endl;
            int i_name = hash_ASCII(name);
            exist_pos = find_model1(i_name);
            if (exist_pos == -1)
                outfile << "-1" << endl;
            else
                outfile << exist_pos << endl;
        }
        else if (opt == 2)
        {
            break;
        }
    }
    */

    //  utf-8测试入口

    infile.open("./data/test_u1.in", ios::in); // test_u1.in    final-data/2-1.in
    outfile.open("./data/test_u1.out", ios::in);
    if (!infile.is_open())
        cout << "Open file failure!" << endl;
    if (!outfile.is_open())
        cout << "Open file failure!" << endl;

    while (!infile.eof()) // 若未到文件结束一直循环
    {
        flag++;
        infile >> opt;
        if (opt == 0)
        {
            infile >> name >> num;

            int i_name = hash_utf8(name);
            // cout << opt << " " << name << " " << num << " " << i_name << endl;
            //   cout << i_name << endl;
            //    exist_pos = find(i_name);
            insert_model3(i_name, num);
        }
        else if (opt == 1)
        {
            infile >> name;
            // cout << opt << " " << name << endl;
            int i_name = hash_utf8(name);
            // cout << opt << " " << name << " " << i_name << endl;
            exist_pos = find_model3(i_name);
            if (exist_pos == -1)
                outfile << "-1" << endl;
            else
                outfile << exist_pos << endl;
        }
        else if (opt == 2)
        {
            break;
        }
    }

    cout << "num of data: " << flag << endl;
    end = clock();
    ret = double(end - begin) / CLOCKS_PER_SEC;
    cout << "runtime:   " << ret << endl;

    infile.close();  // 关闭文件
    outfile.close(); // 关闭文件

    return 0;
}