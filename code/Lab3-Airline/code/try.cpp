#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <stdlib.h>
using namespace std;

int main()
{
    int a[100];
    for (int i = 1; i <= 56; i++)
    {
        cin >> a[i];
    }
    sort(a + 1, a + 57);
    for (int i = 1; i <= 56; i++)
    {
        cout << a[i] << " ";
    }
    return 0;
}