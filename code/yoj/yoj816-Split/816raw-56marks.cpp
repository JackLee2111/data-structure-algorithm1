#include <iostream>
using namespace std;
#define N 1000010
int next1[N];

void create_next(string p)
{
    int m = p.length();
    int t = -1, j = 0;
    next1[0] = -1;
    while (j < m - 1)
    {
        if (t == -1 || p[j] == p[t])
        {
            t++;
            j++;
            if (p[j] != p[t])
                next1[j] = t;
            else
                next1[j] = next1[t];
        }
        else
            t = next1[t];
    }
}
int Count_times(string a, int alo, int ahi, string b)
{

    int n = ahi - alo, i = alo;
    /*cout << "string a is ";
    for (int i = alo; i < ahi; i++)
    {
        cout << a[i];
    }
    cout << endl;
    cout << "string b is " << b << endl;*/

    int m = b.length(), j = 0;
    if (n < m)
        return 0;
    /*if (n == m)
    return (a == b) ? 1 : 0;*/

    int ans = 0;
    while (i <= ahi)
    {
        if (j >= m)
        {
            j = 0;
            ans++;
        }
        if (i >= ahi)
            break;
        // cout << i << endl;

        if (j < 0 || a[i] == b[j])
        {
            i++;
            j++;
        }
        else
        {
            j = next1[j];
        }
    }
    // cout << "ans: " << ans << endl;
    return ans;
}
int main()
{
    string a, b;
    cin >> a >> b;
    int n = a.length();
    int m = b.length();
    int s1 = 0, s2 = 0, ans = 0;

    create_next(b);
    /*for (int i = 0; i <= m + 1; i++)
        cout << next1[i] << " ";
    cout << endl;*/

    int g = Count_times(a, 0, n, b);
    // cout << "g: " << g << endl;

    for (int i = 1; i <= n - 1; i++)
    {
        s1 = Count_times(a, 0, i, b);
        s2 = Count_times(a, i, n, b);
        // cout << "s1: " << s1 << endl;
        // cout << "s2: " << s2 << endl;
        // cout << endl;
        if (g == s1 + s2)
            ans++;
    }
    cout << ans << endl;

    return 0;
}
