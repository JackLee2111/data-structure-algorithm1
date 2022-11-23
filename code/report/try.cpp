#include <iostream>
using namespace std;

int upperBits(int n)
{
    int a = !!(n);
    int b = a << 31;
    int ans = b >> (n + 31);
    /*
        int m = 1 << 31;
        printf("%x\n", m);

        int ans = m >> n;
        printf("%x\n", ans);

        ans = ans << 1;
        printf("%x\n", ans);


        int num = ~0;
        int ans = (num << (32 + ((~n) + 1)))

    */
    return ans;
}

int main()
{
    int n;
    cin >> n;
    printf("%x\n", upperBits(n));
    return 0;
}
