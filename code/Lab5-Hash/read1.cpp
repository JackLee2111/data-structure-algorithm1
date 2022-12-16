#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <stdio.h>
using namespace std;
/*
UTF-8是一种多字节编码的字符集，表示一个Unicode字符时，它可以是1个至多个字节，在表示上有规律：

1字节：0xxxxxxx
2字节：110xxxxx 10xxxxxx
3字节：1110xxxx 10xxxxxx 10xxxxxx
4字节：11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
E0 = 1110 0000
*/
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

bool check2byte(unsigned char c)
{
    // 通过utf-8字节码进行判断
    return c >= 0xE0;
}

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

string int_to_byte(int num)
{

    string s, temp;
    int byte = 0;
    while (num > 0)
    {
        int x = num % 257;
        temp[byte] = char(x);
        byte++;
        num /= 257;
    }
    for (int i = 0; i < byte; i++)
    {
        s[i] = temp[byte - i - 1];
    }
    return s;
}

int code_num[30];
int main()
{
    string str = "的2A df.dasf为 d啊abcdefg";
    // string str = "a sg.abc";的2A df.dasf为 d啊
    //   printf("%s\n", str.c_str());
    cout << str << endl;

    int num = 0;
    for (int i = 0; i < str.size();)
    {
        int byte = get_byte(str[i]);
        char c[4][4];

        strncpy(&c[byte][0], &str[i], byte);
        cout << c[byte] << " ";
        // cout << "byte: " << byte << endl;
        code_num[num] = byte_to_int(c[byte], byte);
        num++;
        // cout << "num: " << num << endl;
        i += byte;
    }
    cout << endl;
    for (int i = 0; i < num; i++)
    {
        cout << code_num[i] << endl;
        // string x = int_to_byte(hash_num[i]);
        // cout << x << endl;
    }
    // printf("大写%d个，小写%d个，数字%d个，字符%d个，汉字%d个，空格%d个\n", UpperCase, LowerCase, digit, character, chinese, space);
    return 0;
}
