#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
// #include <fstream>
using namespace std;
int leftMost = -1, rightMost = 1000008;
int Next[1000010] = {-1};
int ans = 0;
struct Node
{
    int left;
    int right;
    bool covered;
    bool operator<(const struct Node &b)
    {
        if (left != b.left)
        {
            return left < b.left;
        }
        return right < b.right;
    }
    Node(int l, int r) : left(l), right(r) { covered = false; }
};
vector<Node> counter;
vector<int> leftEdge, rightEdge;

int main()
{
    string a, b;
    cin >> a >> b;
    ans = a.length() - 1;
    Next[0] = -1;
    int j = 0;
    int m = b.length();
    int t = -1;
    int l = a.length();
    // 构造next表
    while (j < m)
    {
        (t < 0 || b[j] == b[t]) ? Next[++j] = ++t : t = Next[t];
    }
    int i = 0;
    j = 0;
    while (i < l)
    {
        if (j < 0 || a[i] == b[j])
        {
            i++;
            j++;
        }
        else
            j = Next[j];
        if (j == m)
        {
            counter.push_back(Node(i + 1 - b.size(), i));
            j = Next[j];
        }
    }
    sort(counter.begin(), counter.end());

    int count = counter.size();
    for (int i = 0; i < count; i++)
    {
        if (counter[i].left > leftMost)
        {
            leftEdge.push_back(counter[i].right);
            leftMost = counter[i].right;
        }
        if (counter[count - i - 1].right < rightMost)
        {
            rightEdge.push_back(counter[count - i - 1].left);
            rightMost = counter[count - i - 1].left;
        }
    }
    for (int i = 0; i < leftEdge.size(); i++)
    {
        ans -= leftEdge[i] - rightEdge[rightEdge.size() - i - 1];
    }
    cout << ans;
    return 0;
}