#include <bits/stdc++.h>
using namespace std;
const int N = 1e6;
int lps[N];
int nt[N];
void get_lps(string t, int *lps)
{
    int m = t.size();
    int i = 1, j = 0;
    lps[0] = 0;
    while (i < m)
    {
        if (t[i] == t[j])
        {
            ++j;
            lps[i] = j;
            i++;
        }
        else if (j != 0)
        {
            j = lps[j - 1];
        }
        else
        {
            lps[i] = 0;
            i++;
        }
    }
}
void create_next(string t, int *next)
{
    int m = t.length();
    int k = -1, j = 0;
    next[0] = -1;
    while (j < m - 1)
    {
        if (k == -1 || t[k] == t[j])
        {
            k++;
            j++;
            next[j] = k;
        }
        else
            k = next[k];
    }
}
int Count_times(string a, string b)
{
    int n = a.length();
    int m = b.length();
    int i = 0;
    int j = 0;
    int tot = 0;
    get_lps(a, lps);
    while (i < n && j < m)
    {
        if (a[i] == b[j])
        {
            i++;
            j++;
        }
        else if (j != 0)
            j = lps[j - 1];
        else
            i++;
        if (j == m)
        {
            tot++;
            j = 0;
        }
    }
    return tot;
}
int main()
{
    string a, b;
    cin >> a >> b;
    cout << Count_times(a, b) << endl;
    return 0;
}
