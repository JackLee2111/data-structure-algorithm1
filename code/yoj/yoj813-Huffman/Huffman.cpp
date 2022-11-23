#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

//建树
typedef struct Tree
{
    int freq; // 出现频率
    char key; // 字母
    Tree *left;
    Tree *right;
    Tree(int fr = 0, char k = '\0', Tree *l = nullptr, Tree *r = nullptr) : freq(fr), key(k), left(l), right(r){};
} Tree, *pTree;

// 升序排列
struct cmp
{
    bool operator()(Tree *a, Tree *b)
    {
        return a->freq > b->freq;
    }
};

//字母对应的出现次数和编码
struct letter
{
    char chars;
    int num;
    string code;
};
vector<letter> let;

priority_queue<pTree, vector<pTree>, cmp> prior_que; // 小顶堆

// 利用中序遍历的方法输出霍夫曼编码
// 左0右1，迭代完一次st回退一个字符
void printCode(Tree *proot, string st)
{
    if (proot == nullptr)
    {
        return;
    }

    if (proot->left)
    {
        st += '0';
    }
    printCode(proot->left, st);

    if (!proot->left && !proot->right) // 叶子结点
    {
        string temp = "";
        // printf("%c ", proot->key);
        for (size_t i = 0; i < st.size(); ++i)
        {
            // printf("%c", st[i]);
            temp += st[i];
        }
        int j = 0;
        int flag = 0;
        char c = proot->key;
        int chars_len = let.size();
        for (j = 0; j < chars_len; j++)
        {
            if (c == let[j].chars)
            {
                flag = 1;
                let[j].code = temp;
                break;
            }
        }
    }
    st.pop_back(); // 退回一个字符

    if (proot->right)
    {
        st += '1';
    }
    printCode(proot->right, st);
}

// 清空堆上分配的内存空间
void del(Tree *proot)
{
    if (proot == nullptr)
    {
        return;
    }
    del(proot->left);
    del(proot->right);

    delete proot;
}

// 霍夫曼编码
void huffman_incode()
{

    //将森林中最小的两个频度组成树，放回森林。直到森林中只有一棵树。
    while (prior_que.size() > 1)
    {
        Tree *proot = new Tree;
        pTree pl, pr;
        pl = prior_que.top();
        prior_que.pop();
        pr = prior_que.top();
        prior_que.pop();

        proot->freq = pl->freq + pr->freq;
        proot->left = pl;
        proot->right = pr;

        prior_que.push(proot);
    }

    string str = "";
    printCode(prior_que.top(), str);
    del(prior_que.top());
}

int main()
{
    string str;
    //字符串读入
    getline(cin, str);

    int len = str.length();

    //字母和对应出现次数计算 O(n)
    for (int i = 0; i < len; i++)
    {
        int j = 0;
        int flag = 0;
        char c = str[i];
        int chars_len = let.size();
        for (j = 0; j < chars_len; j++)
        {
            if (c == let[j].chars)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 1)
        {
            let[j].num++;
        }
        else
        {
            letter temp;
            temp.chars = c;
            temp.num = 1;
            temp.code = "";
            let.push_back(temp);
        }
    }

    /*
    for (int i = 0; i < let.size(); i++)
    {
        cout << let[i].chars << ":";
        cout << let[i].num << endl;
    }
    */

    char c;
    int fr;
    int M = let.size(); // 待编码字符个数
    //将字母节点构造进树节点中
    for (int i = 0; i < M; ++i)
    {
        Tree *pt = new Tree;
        pt->key = let[i].chars;
        pt->freq = let[i].num;
        //优先队列
        prior_que.push(pt);
    }

    //哈夫曼编码
    huffman_incode();

    string ans;
    //编码完成后的序列拼接
    for (int i = 0; i < len; i++)
    {
        int j = 0;
        int flag = 0;
        char c = str[i];
        int chars_len = let.size();
        for (j = 0; j < chars_len; j++)
        {
            if (c == let[j].chars)
            {
                flag = 1;
                ans += let[j].code;
                break;
            }
        }
    }
    // cout << ans << endl;
    //编码长度
    int ans_len = ans.length();
    cout << ans_len << endl;

    int l = let.size();

    //每个字母对应的编码
    for (int i = 0; i < l; i++)
    {
        cout << let[i].chars << " ";
        cout << let[i].code << endl;
    }

    return 0;
}
