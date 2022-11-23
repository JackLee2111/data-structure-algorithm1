#include <iostream>
#include <queue>

using namespace std;

//结构体，记录病毒的坐标和第几步被感染的
struct node
{
    int x, y;
    int time;
};
//广搜队列
queue<struct node> q;

char map[1005][1005];
//标记是否是初次到达
int visited[1005][1005];
int n, m;
int dir[4][2] = {0, 1, 0, -1, 1, 0, -1, 0}; //方向数组
int sum = 0;
void bfs()
{

    struct node temp, flag;

    // std::cout << "enter bfs" << endl;
    //当队列为空的时候停止
    while (!q.empty())
    {
        temp = q.front();
        flag = temp;
        q.pop();

        // cout << temp.time << " " << temp.x << " " << temp.y << endl;

        if (visited[temp.x][temp.y] == 0)
        {
            visited[temp.x][temp.y] = 1;
            sum += temp.time; //若为初次到达，则加上感染时间
        }
        //四个方向的比较
        // temp是前一步，flag是走完的一步注意flag的时间是temp + 1
        for (int i = 0; i < 4; i++)
        {
            int tx = temp.x + dir[i][0];
            int ty = temp.y + dir[i][1];
            if (tx < 0 || tx >= n || ty < 0 || ty >= m || map[tx][ty] == '0')
                continue;
            flag.x = tx;
            flag.y = ty;
            flag.time = temp.time + 1;
            q.push(flag);

            map[tx][ty] = '0'; //防止重复搜索
        }
    }
}

int main()
{
    cin >> n >> m;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> map[i][j];
            if (map[i][j] == '0')
            {
                visited[i][j] = 1;
            }
        }
    }

    struct node temp;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            //找病毒位置,入队
            if (map[i][j] == '0')
            {
                temp.x = i;
                temp.y = j;
                temp.time = 0;
                q.push(temp);
                map[i][j] = 1;
            }
        }
    }
    bfs();
    // cout << "sum: " << sum << endl;
    cout << sum << endl;

    return 0;
}
