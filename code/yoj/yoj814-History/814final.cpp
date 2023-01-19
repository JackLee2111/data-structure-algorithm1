#include <iostream>
#include <string>
using namespace std;
#define M 125201
//--------冲突处理策略:线性试探法-----------
// 闭散列节点
struct hashNode
{
    int key;
    int value;
};
struct hashNode Hash_ID2[M + 5];
int probe4free(int key)
{
    int r = key % M;
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
int probe4hit(int key)
{
    int r = key % M;
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
// 冲突处理策略:线性试探法（插入）
void insert_model(int key, int val)
{
    // if (Hash_ID2[probe4hit(key)].key == key) // 已经存在，不必再插入
    //     return;
    int r = probe4free(key);
    Hash_ID2[r].key = key;
    Hash_ID2[r].value = val;
    return;
}
// 冲突处理策略:线性试探法（查找）
int find_model(int key)
{
    int r = probe4hit(key);
    if (Hash_ID2[r].value == 0)
        return -1;
    return Hash_ID2[r].value;
}
void updateTime_model(int key, int time)
{
    int r = probe4hit(key);
    if (Hash_ID2[r].value != 0)
        Hash_ID2[r].value = time; // 更新最新访问时间
    return;
}

int main()
{
    int n, last_ans = 0, time = 1;
    int ask, ans, id, exist_pos;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> ask;
        id = last_ans ^ ask;
        exist_pos = find_model(id); // 查找在哈希表中是否出现过该id，有则返回位置，无则返回-1
        if (exist_pos == -1)        // 首次出现：插入id节点，输出当前时间
        {
            insert_model(id, time);
            ans = time;
        }
        else // 已经存在：输出上次出现时间，更新该id节点最后一次出现时间为当前
        {
            ans = exist_pos;
            updateTime_model(id, time);
        }
        cout << ans << endl;
        last_ans = ans;
        time++;
    }
    return 0;
}