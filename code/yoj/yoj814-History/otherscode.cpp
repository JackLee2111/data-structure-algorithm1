#include <iostream>
using namespace std;
struct keyNum
{
    int key;
    int value;
    struct keyNum *next;
};
struct keyNum *ID_time[133331 + 5];
void insert(int ID, int time)
{
    int hashCode = ID % 133331;
    struct keyNum *temp = ID_time[hashCode], *parent = 0;
    while (temp)
    {
        parent = temp;
        temp = temp->next;
    }
    if (!parent)
    {
        temp = new keyNum;
        temp->key = ID;
        temp->value = time;
        ID_time[hashCode] = temp;
    }
    else
    {
        temp = new keyNum;
        temp->key = ID;
        temp->value = time;
        parent->next = temp;
    }
}

int search(int ID)
{
    int hashCode = ID % 133331;
    struct keyNum *temp = ID_time[hashCode];
    while (temp)
    {
        if (temp->key == ID)
            return temp->value;
        else
            temp = temp->next;
    }
    return -1;
}
void change(int ID, int time)
{
    int hashCode = ID % 133331;
    struct keyNum *temp = ID_time[hashCode];
    while (temp)
    {
        if (temp->key == ID)
        {
            temp->value = time;
            return;
        }
        else
        {
            temp = temp->next;
        }
    }
}

int timeNow = 1;
int main()
{
    int n, lastAnswer = 0, temp, answer, iter;
    cin >> n;
    for (int i = 0; i < n; i++, timeNow++)
    {
        cin >> temp;
        iter = search(lastAnswer ^ temp);
        if (iter != -1)
        {
            // cout << (lastAnswer ^ temp) << ' ' << timeNow << ' ';
            answer = iter;
            // ID_time[lastAnswer ^ temp] = timeNow;
            change(lastAnswer ^ temp, timeNow);
        }
        else
        {
            insert(lastAnswer ^ temp, timeNow);
            answer = timeNow;
        }
        cout << answer << endl;
        lastAnswer = answer;
    }
    return 0;
}