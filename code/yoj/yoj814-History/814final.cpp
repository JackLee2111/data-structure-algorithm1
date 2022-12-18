#include <iostream>
#include <string>
using namespace std;

// 节点
struct hashNode
{
    int key;
    int value;
    struct hashNode *next = NULL;
};
struct hashNode *Hash_ID[125209];

void insert(int id, int time)
{
    int hashCode = id % 125201;
    struct hashNode *now = Hash_ID[hashCode];
    struct hashNode *pre = 0;
    while (now) // 找到最末尾的节点
    {
        pre = now;
        now = now->next;
    }

    now = new hashNode;
    if (pre) // 待插入节点冲突
    {
        now->key = id;
        now->value = time;
        pre->next = now; // 将now接到pre后面，形成查找链
    }
    else // 要插入的节点未冲突
    {
        now->key = id;
        now->value = time;
        Hash_ID[hashCode] = now;
    }
}

int find(int id)
{
    int hashCode = id % 125201;
    struct hashNode *now = Hash_ID[hashCode];
    while (now)
    {
        if (now->key == id)
            return now->value;
        else
            now = now->next;
    }
    return -1;
}
void updateTime(int id, int time)
{
    int hashCode = id % 125201;
    struct hashNode *now = Hash_ID[hashCode];
    while (now)
    {
        if (now->key == id) // 找到该id的节点，改变最后一次更新的值
        {
            now->value = time;
            return;
        }
        else
            now = now->next;
    }
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
        exist_pos = find(id); // 查找在哈希表中是否出现过该id，有则返回位置，无则返回-1
        if (exist_pos == -1)  // 首次出现：插入id节点，输出当前时间
        {
            insert(id, time);
            ans = time;
        }
        else // 已经存在：输出上次出现时间，更新该id节点最后一次出现时间为当前
        {
            ans = exist_pos;
            updateTime(id, time);
        }
        cout << ans << endl;
        last_ans = ans;
        time++;
    }
    return 0;
}