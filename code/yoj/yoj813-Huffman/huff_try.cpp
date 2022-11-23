#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;
vector<char> chars;
vector<int> num;

int main()
{
    string str;
    getline(cin, str);

    int len = str.length();

    for (int i = 0; i < len; i++)
    {
        int j = 0;
        int flag = 0;
        char c = str[i];
        int chars_len = chars.size();
        for (j = 0; j < chars_len; j++)
        {
            if (c == chars[j])
            {
                flag = 1;
                break;
            }
        }
        if (flag == 1)
        {
            num[j]++;
        }

        else
        {
            chars.push_back(c);
            num.push_back(0);
            num[chars_len]++;
        }
    }

    for (int i = 0; i < chars.size(); i++)
    {
        cout << chars[i] << ":";
        cout << num[i] << endl;
    }

    return 0;
}

/* 读入测试数据
 *   a 45
 *   b 13
 *   c 12
 *   d 16
 *   e 9
 *   f 5
 */