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
    if (c >> 7 == 0)
        return 1;
    else if (c >> 5 == 0x6)
        return 2;
    else if (c >> 4 == 0xE)
        return 3;
    else if (c >> 4 == 0xF)
        return 4;
}
bool check2byte(unsigned char c)
{
    // 通过utf-8字节码进行判断
    return c >= 0xE0;
}
int main()
{
    string str = " 的2Adf.dasf为 d啊 ";
    printf("%s\n", str.c_str());
    cout << str << endl;
    int LowerCase, UpperCase; // 大写，小写
    int space = 0;
    int digit, character; // 数字，字符
    int chinese = 0;      // 中文
    digit = character = LowerCase = UpperCase = 0;
    for (int i = 0; i < str.size();)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            LowerCase++;
            i++;
        }
        else if (str[i] >= 'A' && str[i] <= 'Z')
        {
            UpperCase++;
            i++;
        }
        else if (str[i] >= '0' && str[i] <= '9')
        {
            digit++;
            i++;
        }
        else if (get_byte(str[i]) == 3)
        {
            char c[3];
            strncpy(&c[0], &str[i], 3);
            cout << c << endl;
            i += 3;
            chinese++;
        }
        else if (str[i] == ' ')
        {
            space++;
            i++;
        }
        else
        {
            character++;
            i++;
        }
    }
    printf("大写%d个，小写%d个，数字%d个，字符%d个，汉字%d个，空格%d个\n", UpperCase, LowerCase, digit, character, chinese, space);
    return 0;
}
