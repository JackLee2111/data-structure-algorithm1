

// 节点

/*
struct hashNode
{
    int key;
    int value;
    struct hashNode *next = NULL;
};
struct hashNode *Hash_ID[125209];
void insert(int id, int time)
{
    int hashCode = id % M;
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
    int hashCode = id % M;
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
    int hashCode = id % M;
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
*/