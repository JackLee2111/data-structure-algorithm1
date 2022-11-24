// 一些不存在情况的特判（-1）
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <list>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

//航班数据，来自data.csv
struct Airline_data
{
    int airline_id;        //航班序号
    string departure_date; //出发时间 ->int
    int depart_date_i;     //出发时间 int类型 例如 57 代表5月7日
    string inOrDo;         //国际、国内航班 -> 01
    int flight_num;        //航班号
    int depart_airport;    //出发机场
    int arriv_airport;     //到达机场

    string depart_time;
    int depart_time_i; //->int,例如 571220 表示5.7-12:20 570630
    string arriv_time;
    int arriv_time_i; //->int,例如 571220

    int airplane_id;
    int airplane_model; //机型
    int fare;
};
//简化版航班数据
struct air
{
    int from;
    int to;
};
Airline_data Airline_temp0;
vector<struct Airline_data> AirlineVector; //航班数据存储

vector<struct air> maxstep_airline; //最大航次下的可达性

// task1邻接表
#define VertexType int
#define VERTEX_NUM 82
#define EDGE_NUM 2347
int VERTEX = 79;
int EDGE = 2346;

int visit_dfs[VERTEX_NUM]; //记录dfs遍历
int visit_bfs[VERTEX_NUM]; //记录bfs遍历

// task2邻接矩阵
bool visited[80] = {0};
const int MaxSize = 80; //最大顶点数

// task2 邻接矩阵实现——————————————————————————————————————————————————————
template <class DataType>
class MatrixGraph
{
public:
    MatrixGraph(DataType a[], int n, int e, int mode);
    void Output();
    void DFS(int start, int v, int step, int maxstep); //最大步数搜索
    void DFS_unlimited(int start, int v);              //无限制搜索
    void BFS(int v, int maxstep);
    int get_arcij(int from, int to);

private:
    DataType vertex[MaxSize];       //顶点数组
    int arc[MaxSize][MaxSize];      //邻接矩阵
    int time_arc[MaxSize][MaxSize]; //考虑时序关系的邻接矩阵
    int vertexNum, edgeNum;         //图中当前的顶点数与边数
};

template <class DataType>
MatrixGraph<DataType>::MatrixGraph(DataType a[], int n, int e, int mode) // mode1为初次构造，mode2为根据maxstep构造
{
    int i, j, k;
    vertexNum = n;
    edgeNum = e;
    for (i = 0; i <= vertexNum; i++)
        vertex[i] = a[i];            //顶点数组赋值
    for (i = 0; i <= vertexNum; i++) //初始化邻接矩阵
        for (j = 0; j <= vertexNum; j++)
            arc[i][j] = 0;
    if (mode == 1)
    {
        for (k = 1; k <= edgeNum; k++) //依次输入每一条边
        {
            // cin >> i >> j; //输入边依附的两个顶点的编号
            int start = AirlineVector[k].depart_airport;
            int end = AirlineVector[k].arriv_airport;

            arc[start][end]++;
            // arc[j][i] = 1; //无向图
        }
    }
    else if (mode == 2)
    {
        for (k = 0; k < maxstep_airline.size(); k++)
        {
            int start = maxstep_airline[k].from;
            int end = maxstep_airline[k].to;

            arc[start][end]++;
        }
    }
}

//输出邻接矩阵函数
template <class DataType>
void MatrixGraph<DataType>::Output()
{
    int i, j;
    for (i = 1; i <= vertexNum; i++)
    { //输出邻接矩阵
        for (j = 1; j <= vertexNum; j++)
        {
            if (arc[i][j] > 1)
            {
                cout << 1 << " ";
            }
            else
                cout << arc[i][j] << " ";
        }
        cout << endl;
    }
    // cout << "_________" << endl;
    // cout << arc[49][39] << endl;
}

//输出邻接矩阵的某个点
template <class DataType>
int MatrixGraph<DataType>::get_arcij(int from, int to)
{
    return arc[from][to];
    // cout << arc[49][39] << endl;
}
template <class DataType>
void MatrixGraph<DataType>::DFS_unlimited(int start, int v)
{

    air airline;
    airline.from = start;
    airline.to = v;
    maxstep_airline.push_back(airline);
    // cout << vertex[v] << " "; //输出顶点的值

    visited[v] = true;                   //标记被访问
    for (int j = 1; j <= vertexNum; j++) // j 小于顶点数时
    {
        if (arc[v][j] >= 1 && visited[j] == false)
        {
            visited[j] = true;
            DFS_unlimited(start, j);
            visited[j] == false;
        }
    }
}

template <class DataType>
void MatrixGraph<DataType>::DFS(int start, int v, int step, int maxstep)
{
    if (step > maxstep)
        return;
    else if (step < maxstep)
    {
        // cout << "now " << start << " step is " << step << " v is " << v << endl;
        air airline;
        airline.from = start;
        airline.to = v;
        maxstep_airline.push_back(airline);
        // cout << vertex[v] << " "; //输出顶点的值
    }
    else if (step == maxstep)
    {
        // cout << "now " << start << " step is " << step << " v is " << v << endl;
        air airline;
        airline.from = start;
        airline.to = v;
        maxstep_airline.push_back(airline);
        return;
    }
    visited[v] = true;                   //标记被访问
    for (int j = 1; j <= vertexNum; j++) // j 小于顶点数时
    {
        if (arc[v][j] == 0 || visited[j] == true)
            continue;

        visited[j] == true;
        step++;
        DFS(start, j, step, maxstep);
        step--;
        visited[j] == false;
        /*int flights_num = arc[v][j];
        for (int w = 1; w <= flights_num; w++)
        {}*/
    }
}

int time_1trans_air[80][80] = {0}; //考虑时序，1次转机的可达性（task3）
// task3构建 考虑时序的1次转机可达性航线统计
void task3_find_time_1trans_airline()
{
    for (int i = 1; i <= EDGE; i++)
    {
        int start_ap = AirlineVector[i].depart_airport;
        int trans_ap = AirlineVector[i].arriv_airport;
        int trans_time_1 = AirlineVector[i].arriv_time_i; //第一班飞机到达时间

        for (int j = 1; j <= EDGE; j++)
        {
            if (AirlineVector[j].depart_airport != trans_ap) //第一班终点不为第二班起点，不符合
                continue;
            int trans_time_2 = AirlineVector[j].depart_time_i;
            if (trans_time_1 > trans_time_2) //第一班到达时间比第二版出发时间晚，不符合
                continue;
            int end_ap = AirlineVector[j].arriv_airport;

            time_1trans_air[start_ap][end_ap]++;
        }
    }
    return;
}

template <class DataType>
void MatrixGraph<DataType>::BFS(int v, int maxstep) //传入顶点序号
{
    bool visited[80] = {0};
    queue<int> Q;             //初始化顺序队列
    cout << vertex[v] << " "; //输出顶点
    visited[v] = true;        //标记被访问
    Q.push(v);                //顶点v入队列
    while (!Q.empty())        //当队列非空时
    {
        v = Q.front(); //将队头元素取出送到v中
        Q.pop();
        for (int j = 1; j <= vertexNum; j++)
        { //小于顶点个数时
            if (arc[v][j] == 1 && visited[j] == false)
            {
                cout << vertex[j] << " ";
                visited[j] = true;
                Q.push(j);
            }
        }
    }
}

// task1 邻接表实现——————————————————————————————————————————————————————
//边表结点
typedef struct EdgeNode
{
    int adjvex;     //索引值是边表节点自己在数组中的索引值，而不是和它连接顶点的下标。
    EdgeNode *next; //下一条边指向的顶点
    int weight;     //有权图
} EdgeNode;

//顶点表结点
typedef struct VertexNode
{
    VertexType airport_idd;
    EdgeNode *firstedge; //第一条边指向的顶点
} AdjList[VERTEX_NUM];

//图
typedef struct
{
    AdjList adjList;
    int maxVertexes, numEdge;
} ALGraph;

// task4-6 邻接表 1 begin————————————————————————————————————————————————————————
//边表结点
typedef struct ENode
{
    int airport_id; //索引值，即机场编号

    int depart_time;
    int arrive_time;
    int fare;
    int model;

    int time;
    int weight; //有权图
} ENode;
//顶点表结点
typedef struct VNode
{
    VertexType airport_id;
    vector<ENode> Node;
} AList;
//图
typedef struct
{
    AList aList[VERTEX_NUM];
    int Vertex, Edge;
} AListGraph;

// 建立机场邻接表,方式2
void CreateAL2(AListGraph *g)
{
    int end, start;
    g->Vertex = 79; //节点数
    g->Edge = 2346; //边数

    //每个顶点保存的数据
    for (int i = 0; i <= 79; i++)
    {
        g->aList[i].airport_id = i; //第0个机场为1
    }

    //每条边的两个顶点
    for (int i = 1; i <= 2346; i++)
    {
        // cout << "请输入第" << j << "条边:" << endl;
        // AirlineVector[j + 1].arriv_airport , AirlineVector[j + 1].depart_airport;

        start = AirlineVector[i].depart_airport;
        end = AirlineVector[i].arriv_airport;

        int fare = AirlineVector[i].fare;
        int model = AirlineVector[i].airplane_model;
        int dtime = AirlineVector[i].depart_time_i;
        int atime = AirlineVector[i].arriv_time_i;

        ENode temp;
        temp.airport_id = end;
        temp.arrive_time = atime;
        temp.depart_time = dtime;
        temp.fare = fare;
        temp.model = model;

        g->aList[start].Node.push_back(temp); //加入以start为起点的邻接表中
    }
}

void PrintLinkAL2(AListGraph *g, AList node);

void dispAL2(AListGraph *g)
{
    cout << "邻接表为:" << endl;
    for (int i = 1; i <= 79; i++)
    {
        cout << g->aList[i].airport_id << ": ";
        PrintLinkAL2(g, g->aList[i]);
    }
}
void PrintLinkAL2(AListGraph *g, AList node)
{
    int num = node.Node.size();
    for (int i = 0; i < num; i++)
    {
        cout << node.Node[i].airport_id << " ";
    }
    cout << endl;
}

// task4-8 航班为节点邻接表 begin————————————————————————————————————————————————————————
//边表结点
typedef struct ENode3
{
    int ap_from;
    int ap_to;
    int airline_id;

    int depart_time;
    int arrive_time;
    int fare;
    int model;

} ENode3;
//顶点表结点
typedef struct VNode3
{
    int ap_from;
    int ap_to;
    int airline_id;

    int depart_time;
    int arrive_time;
    int fare;
    int model;

    vector<ENode3> Node;
} AList3;
//图
typedef struct
{
    AList3 aList[EDGE_NUM];
    int Vertex, Edge;
} AL3;

// task4-8 建立航线邻接表
void CreateAL3(AL3 *g)
{
    g->Vertex = 2346; //航班数

    //每个顶点保存的数据
    for (int i = 0; i <= 2346; i++)
    {
        g->aList[i].airline_id = i;
    }

    //每条边的两个顶点
    for (int i = 1; i <= 2346; i++)
    {
        AList3 vnode;
        int start_1 = AirlineVector[i].depart_airport;
        int end_1 = AirlineVector[i].arriv_airport;
        int airline_id_1 = AirlineVector[i].airline_id;
        int fare_1 = AirlineVector[i].fare;
        int model_1 = AirlineVector[i].airplane_model;
        int dtime_1 = AirlineVector[i].depart_time_i;
        int atime_1 = AirlineVector[i].arriv_time_i;

        vnode.ap_from = start_1;
        vnode.ap_to = end_1;
        vnode.airline_id = airline_id_1;
        vnode.fare = fare_1;
        vnode.model = model_1;
        vnode.depart_time = dtime_1;
        vnode.arrive_time = atime_1;

        for (int j = 1; j <= 2346; j++)
        {
            int start_2 = AirlineVector[j].depart_airport;

            if (end_1 != start_2) //上一班的终点不为该班的起点
                continue;

            int dtime_2 = AirlineVector[j].depart_time_i;
            int atime_2 = AirlineVector[j].arriv_time_i;

            if (dtime_2 < atime_1) //上一班的到达比该班起飞还要晚
                continue;

            int end_2 = AirlineVector[j].arriv_airport;
            int airline_id_2 = AirlineVector[j].airline_id;

            int fare_2 = AirlineVector[j].fare;
            int model_2 = AirlineVector[j].airplane_model;

            ENode3 enode;

            enode.ap_from = start_2;
            enode.ap_to = end_2;
            enode.airline_id = airline_id_2;
            enode.fare = fare_2;
            enode.model = model_2;
            enode.depart_time = dtime_2;
            enode.arrive_time = atime_2;

            vnode.Node.push_back(enode);
        }
        // cout << "请输入第" << j << "条边:" << endl;
        // AirlineVector[j + 1].arriv_airport , AirlineVector[j + 1].depart_airport;

        g->aList[i] = vnode; //加入以i为起点的邻接表中
    }
}

void PrintLinkAL3(AL3 *g, AList3 node);

void dispAL3(AL3 *g)
{
    cout << "航线邻接表为:" << endl;
    for (int i = 1; i <= 20; i++)
    {
        cout << g->aList[i].airline_id << ": ";
        PrintLinkAL3(g, g->aList[i]);
    }
}
void PrintLinkAL3(AL3 *g, AList3 node)
{
    int num = node.Node.size();
    for (int i = 0; i < num; i++)
    {
        cout << node.Node[i].airline_id << " ";
    }
    cout << endl;
}

// task4 time_min dijkstra ————————————————————————————————————————
const int inf = 8888888;

int d[3010], vis[3010] = {0}; // d为当前航班到起点航班的最短时间（已更新），vis为是否结束访问
int n, m;
int s, t;
struct t4_dis //优先队列的结构
{
    int airline_id, arriv_time;
    t4_dis(int airline_id, int arriv_time) : airline_id(airline_id), arriv_time(arriv_time) {}
    bool operator<(const t4_dis &a) const
    {
        return arriv_time > a.arriv_time;
    }
};
void t4_init()
{
    for (int i = 1; i <= 3000; i++)
        d[i] = inf;
    memset(vis, 0, sizeof(vis));
}

priority_queue<t4_dis> t4_q;
void t4_dij(AL3 *g, int start)
{
    t4_q.push(t4_dis(start, 0)); //起点入队，时间为0
    d[start] = 0;                //最小距离数组为0

    while (!t4_q.empty()) //优先队列非空
    {
        t4_dis a = t4_q.top(); //取出时间最小的点
        t4_q.pop();

        int now_id = a.airline_id, min_time = a.arriv_time; //最小点的id和距离
        vis[now_id] = 1;                                    //取出来设定为已经访问

        // cout << "now id " << now_id << endl;

        int num = g->aList[now_id].Node.size();
        for (int i = 0; i < num; i++) //遍历该航班的所有可达续航班
        {
            int time_val = g->aList[now_id].Node[i].arrive_time;
            // cout << i << " time " << time_val << " airline " << g->aList[now_id].Node[i].airline_id << endl;
            int to = g->aList[now_id].Node[i].airline_id;
            // cout << "to " << to << endl;
            // cout << "dto " << d[to] << endl;
            if (!vis[to] && time_val < d[to])
            {
                // cout << "enter push" << endl;
                d[to] = time_val;
                t4_q.push(t4_dis(to, d[to]));
            }
        }
    }
    /*
    if (d[end] != inf)
        return d[end]; //起点到终点的最短路径
    else
        return -1;*/
}

int cal_during_time(int t1, int t2) //计算时间差
{
    int month1 = t1 / 100000;
    int day1 = (t1 / 10000) % 10;
    int hour1 = (t1 / 100) % 100;
    int min1 = t1 % 100;

    // cout << month1 << " " << day1 << " " << hour1 << " " << min1 << endl;

    int month2 = t2 / 100000;
    int day2 = (t2 / 10000) % 10;
    int hour2 = (t2 / 100) % 100;
    int min2 = t2 % 100;

    int d_time = (day2 - day1) * 1440 + (hour2 - hour1) * 60 + (min2 - min1);
    return d_time;
}

// task5  dijkstra ————————————————————————————————————————

int d5[3010], vis5[3010] = {0}; // d为当前航班到起点航班的最短时间（已更新），vis为是否结束访问
int path5[10005];
int trans[3010]; //记录到该航班的中转次数

struct t5_dis //中转次数为优先级
{
    int airline_id, trans_times;
    t5_dis(int airline_id, int trans_times) : airline_id(airline_id), trans_times(trans_times) {}
    bool operator<(const t5_dis &a) const
    {
        return trans_times > a.trans_times;
    }
};
void t5_init()
{
    for (int i = 1; i <= 3000; i++)
        d5[i] = inf;
    memset(vis5, 0, sizeof(vis5));
}

void t5_print_path(int path[], int dest)
{
    if (path[dest] == -1)
        printf("%d", dest);
    else
    {
        t5_print_path(path, path[dest]);
        printf("->%d", dest);
    }
}

priority_queue<t5_dis> t5_q;

void t5_dij(AL3 *g, int start, int k_max, int mode, int model)
{
    t5_q.push(t5_dis(start, 0)); //起点入队，中转次数为0
    d5[start] = 0;               //中转次数数组为0

    for (int i = 1; i <= 10000; i++) //路径数组
        path5[i] = -1;

    while (!t5_q.empty()) //优先队列非空
    {
        t5_dis a = t5_q.top(); //取出时间最小的点
        t5_q.pop();

        int now_id = a.airline_id, min_trans = a.trans_times; //最小点的id和距离
        vis5[now_id] = 1;                                     //取出来设定为已经访问

        // cout << "now id " << now_id << endl;

        int num = g->aList[now_id].Node.size();
        // cout << "num: " << num << endl;
        for (int i = 0; i < num; i++) //遍历该航班的所有可达续航班
        {
            int to = g->aList[now_id].Node[i].airline_id;
            if (vis5[to] == 1)
                continue;
            trans[to] = trans[now_id] + 1;
            int trans_val = trans[to];

            if (mode == 1 || mode == 2) // 1、2两种模式
            {
                if (trans_val < d5[to] && trans_val <= k_max) //对中转次数进行剪枝
                {
                    // cout << "enter push" << endl;
                    d5[to] = trans_val;
                    t5_q.push(t5_dis(to, d5[to]));
                    path5[to] = now_id;
                }
            }
            else if (mode == 3) //机型限定模式
            {
                int temp_model = g->aList[now_id].Node[i].model;
                if (trans_val < d5[to] && trans_val <= k_max && temp_model == model) //对中转次数进行剪枝
                {
                    // cout << "enter push" << endl;
                    d5[to] = trans_val;
                    t5_q.push(t5_dis(to, d5[to]));
                    path5[to] = now_id;
                }
            }
        }
    }
}

// task6  fare_min dijkstra ————————————————————————————————————————
int d6[3010], vis6[3010] = {0}; // d为当前航班到起点航班的最短时间（已更新），vis为是否结束访问
int path6[1000005];
struct t6_dis //中转次数为优先级
{
    int airline_id, fare;
    t6_dis(int airline_id, int fare) : airline_id(airline_id), fare(fare) {}
    bool operator<(const t6_dis &a) const
    {
        return fare > a.fare;
    }
};
void t6_init()
{
    for (int i = 1; i <= 3000; i++)
        d6[i] = inf;
    memset(vis6, 0, sizeof(vis6));
}

void t6_print_path(int path[], int dest)
{
    if (path[dest] == -1)
        printf("%d", dest);
    else
    {
        t6_print_path(path, path[dest]);
        printf("->%d", dest);
    }
}

priority_queue<t6_dis> t6_q;

void t6_dij(AL3 *g, int start, int mode, int model) // dij
{
    t6_q.push(t6_dis(start, g->aList[start].fare)); //起点入队，费用为当前飞机的fare
    d6[start] = g->aList[start].fare;               //最小费用数组为当前飞机的fare

    for (int i = 1; i <= 10000; i++) //路径数组
        path6[i] = -1;

    while (!t6_q.empty()) //优先队列非空
    {
        t6_dis a = t6_q.top(); //取出费用最少的点
        t6_q.pop();

        int now_id = a.airline_id, min_fare = a.fare; //该点的费用
        vis6[now_id] = 1;                             //取出来设定为已经访问

        int num = g->aList[now_id].Node.size();
        for (int i = 0; i < num; i++) //遍历该航班的所有可达续航班
        {
            int fare_val = g->aList[now_id].Node[i].fare;
            //  cout << i << " time " << time_val << " airline " << g->aList[now_id].Node[i].airline_id << endl;
            int to = g->aList[now_id].Node[i].airline_id;
            // cout << "to " << to << endl;
            // cout << "dto " << d[to] << endl;
            if (mode == 1 || mode == 2) // 1、2两种模式
            {
                if (!vis6[to] && fare_val + min_fare < d6[to])
                {
                    // cout << "enter push" << endl;
                    d6[to] = fare_val + min_fare;
                    t6_q.push(t6_dis(to, d6[to]));
                    path6[to] = now_id;
                }
            }
            else if (mode == 3) //机型限定模式
            {
                int temp_model = g->aList[now_id].Node[i].model;
                if (!vis6[to] && fare_val + min_fare < d6[to] && temp_model == model)
                {
                    // cout << "enter push" << endl;
                    d6[to] = fare_val + min_fare;
                    t6_q.push(t6_dis(to, d6[to]));
                    path6[to] = now_id;
                }
            }
        }
    }
}
// task7  dijkstra ————————————————————————————————————————

int d7[3010], vis7[3010] = {0}; // d为当前航班到起点航班的最短时间（已更新），vis为是否结束访问
int path7[10007];

struct t7_dis //中转次数为优先级
{
    int airline_id, trans_times;
    t7_dis(int airline_id, int trans_times) : airline_id(airline_id), trans_times(trans_times) {}
    bool operator<(const t7_dis &a) const
    {
        return trans_times > a.trans_times;
    }
};
void t7_init()
{
    for (int i = 1; i <= 3000; i++)
        d7[i] = inf;
    memset(vis7, 0, sizeof(vis7));
}

void t7_print_path(int path[], int dest)
{
    if (path[dest] == -1)
        printf("%d", dest);
    else
    {
        t7_print_path(path, path[dest]);
        printf("->%d", dest);
    }
}

priority_queue<t7_dis> t7_q;
int sum_trans_time[3000] = {0}; //记录总中转时间

void t7_dij(AL3 *g, int start, int k_max, int mode, int maxdtime)
{
    t7_q.push(t7_dis(start, 0)); //起点入队，中转次数为0
    d7[start] = 0;               //中转次数数组为0

    for (int i = 1; i <= 10000; i++) //路径数组
        path7[i] = -1;

    while (!t7_q.empty()) //优先队列非空
    {
        t7_dis a = t7_q.top(); //取出时间最小的点
        t7_q.pop();

        int now_id = a.airline_id, min_trans = a.trans_times; //最小点的id和距离
        vis7[now_id] = 1;                                     //取出来设定为已经访问

        int num = g->aList[now_id].Node.size();
        // cout << "num: " << num << endl;
        for (int i = 0; i < num; i++) //遍历该航班的所有可达续航班
        {
            int to = g->aList[now_id].Node[i].airline_id;
            if (vis7[to] == 1)
                continue;
            trans[to] = trans[now_id] + 1;
            int trans_val = trans[to];

            int d_time = cal_during_time(AirlineVector[now_id].arriv_time_i, AirlineVector[to].depart_time_i);

            if (mode == 1) // 单次中转时间
            {
                if (trans_val < d7[to] && trans_val <= k_max && d_time <= maxdtime) //对中转次数进行剪枝
                {
                    // cout << "enter push" << endl;
                    d7[to] = trans_val;
                    t7_q.push(t7_dis(to, d7[to]));
                    path7[to] = now_id;
                }
            }
            else if (mode == 2) //总计中转时间
            {
                sum_trans_time[to] = sum_trans_time[now_id] + d_time;
                if (trans_val < d7[to] && trans_val <= k_max && sum_trans_time[to] <= maxdtime) //对中转次数进行剪枝
                {
                    // cout << "enter push" << endl;
                    d7[to] = trans_val;
                    t7_q.push(t7_dis(to, d7[to]));
                    path7[to] = now_id;
                }
            }
        }
    }
}

// task8  fare_min dijkstra ————————————————————————————————————————
int d8[3010], vis8[3010] = {0}; // d为当前航班到起点航班的最短时间（已更新），vis为是否结束访问
int path8[1000005];
struct t8_dis //中转次数为优先级
{
    int airline_id, fare;
    t8_dis(int airline_id, int fare) : airline_id(airline_id), fare(fare) {}
    bool operator<(const t8_dis &a) const
    {
        return fare > a.fare;
    }
};
void t8_init()
{
    for (int i = 1; i <= 3000; i++)
        d8[i] = inf;
    memset(vis8, 0, sizeof(vis8));
}

void t8_print_path(int path[], int dest)
{
    if (path[dest] == -1)
        printf("%d", dest);
    else
    {
        t8_print_path(path, path[dest]);
        printf("->%d", dest);
    }
}

priority_queue<t8_dis> t8_q;

void t8_dij(AL3 *g, int start, int mode, int maxdtime) // dij
{
    t8_q.push(t8_dis(start, g->aList[start].fare)); //起点入队，费用为当前飞机的fare
    d8[start] = g->aList[start].fare;               //最小费用数组为当前飞机的fare

    for (int i = 1; i <= 10000; i++) //路径数组
        path8[i] = -1;

    while (!t8_q.empty()) //优先队列非空
    {
        t8_dis a = t8_q.top(); //取出费用最少的点
        t8_q.pop();

        int now_id = a.airline_id, min_fare = a.fare; //该点的费用
        vis8[now_id] = 1;                             //取出来设定为已经访问

        int num = g->aList[now_id].Node.size();
        for (int i = 0; i < num; i++) //遍历该航班的所有可达续航班
        {
            int fare_val = g->aList[now_id].Node[i].fare;
            //  cout << i << " time " << time_val << " airline " << g->aList[now_id].Node[i].airline_id << endl;
            int to = g->aList[now_id].Node[i].airline_id;
            // cout << "to " << to << endl;
            int d_time = cal_during_time(AirlineVector[now_id].arriv_time_i, AirlineVector[to].depart_time_i);
            if (mode == 1) // 单次中转时间
            {
                if (!vis8[to] && fare_val + min_fare < d8[to] && d_time <= maxdtime)
                {
                    // cout << "enter push" << endl;
                    d8[to] = fare_val + min_fare;
                    t8_q.push(t8_dis(to, d8[to]));
                    path8[to] = now_id;
                }
            }

            else if (mode == 2) //总中转时间
            {
                sum_trans_time[to] = sum_trans_time[now_id] + d_time;
                if (!vis8[to] && fare_val + min_fare < d8[to] && sum_trans_time[to] <= maxdtime)
                {
                    // cout << "enter push" << endl;
                    d8[to] = fare_val + min_fare;
                    t8_q.push(t8_dis(to, d8[to]));
                    path8[to] = now_id;
                }
            }
        }
    }
}

//对航线的初筛
vector<int> air_id_from_ap;
void t4_get_airline_from_ap(int airport) //获取该机场出发的所有航班序号
{
    for (int i = 1; i <= 2346; i++)
    {
        if (AirlineVector[i].depart_airport == airport)
            air_id_from_ap.push_back(AirlineVector[i].airline_id);
    }
}
void t51_get_airline_from_ap(int airport, int mintime, int maxtime) //该机场出发的所有航班序号，加上出发时间特判
{
    for (int i = 1; i <= 2346; i++)
    {
        if (AirlineVector[i].depart_airport == airport && AirlineVector[i].depart_time_i >= mintime && AirlineVector[i].depart_time_i <= maxtime)
            air_id_from_ap.push_back(AirlineVector[i].airline_id);
    }
}
void t52_get_airline_from_ap(int airport) //获取该机场出发的所有航班序号
{
    for (int i = 1; i <= 2346; i++)
    {
        if (AirlineVector[i].depart_airport == airport)
            air_id_from_ap.push_back(AirlineVector[i].airline_id);
    }
}
void t53_get_airline_from_ap(int airport, int model) //获取该机场出发的所有航班序号，机型满足
{
    for (int i = 1; i <= 2346; i++)
    {
        if (AirlineVector[i].depart_airport == airport && AirlineVector[i].airplane_model == model)
            air_id_from_ap.push_back(AirlineVector[i].airline_id);
    }
}
vector<int> air_id_to_ap;
void t4_get_airline_to_ap(int airport) //获取该机场出发的所有航班序号
{
    for (int i = 1; i <= 2346; i++)
    {
        if (AirlineVector[i].arriv_airport == airport)
            air_id_to_ap.push_back(AirlineVector[i].airline_id);
    }
}
void t51_get_airline_to_ap(int airport) //获取该机场出发的所有航班序号
{
    for (int i = 1; i <= 2346; i++)
    {
        if (AirlineVector[i].arriv_airport == airport)
            air_id_to_ap.push_back(AirlineVector[i].airline_id);
    }
}
void t52_get_airline_to_ap(int airport, int mintime, int maxtime) //获取该机场出发的所有航班序号，加上到达时间特判
{
    for (int i = 1; i <= 2346; i++)
    {
        if (AirlineVector[i].arriv_airport == airport && AirlineVector[i].arriv_time_i >= mintime && AirlineVector[i].arriv_time_i <= maxtime)
            air_id_to_ap.push_back(AirlineVector[i].airline_id);
    }
}
void t53_get_airline_to_ap(int airport, int model) //获取该机场出发的所有航班序号，机型满足
{
    for (int i = 1; i <= 2346; i++)
    {
        if (AirlineVector[i].arriv_airport == airport && AirlineVector[i].airplane_model == model)
            air_id_to_ap.push_back(AirlineVector[i].airline_id);
    }
}

void PrintLink(ALGraph *g, EdgeNode *next);
void CreateAL1(ALGraph *g);
void disp(ALGraph *g);
void DFS(ALGraph *G, int i);
void task1_dfs_find(ALGraph *G, int i);
void DFSTraverse(ALGraph *G);

int turn_time_to_int(string str) //时间由string转为int类型
{
    int time;
    if (str[10] == ':')
        time = (str[9] - 48) * 100 + (str[11] - 48) * 10 + (str[12] - 48);
    else if (str[11] == ':')
        time = (str[9] - 48) * 1000 + (str[10] - 48) * 100 + (str[12] - 48) * 10 + (str[13] - 48);
    time += (str[0] - 48) * 100000 + (str[2] - 48) * 10000;
    return time;
}

//数据读入
void ReadData()
{
    /*读入data.CSV文件*/
    ifstream infile("./data.csv", ios::in);
    string line;

    getline(infile, line);
    AirlineVector.push_back(Airline_temp0); //塞一个空东西进去 [0]=0
    while (getline(infile, line))
    {
        stringstream ss(line);
        string str;
        Airline_data Airline_temp;

        getline(ss, str, ',');
        Airline_temp.airline_id = stod(str);
        getline(ss, str, ',');
        Airline_temp.departure_date = str;
        getline(ss, str, ',');
        Airline_temp.inOrDo = str;
        getline(ss, str, ',');
        Airline_temp.flight_num = stod(str);
        getline(ss, str, ',');
        Airline_temp.depart_airport = stod(str);
        getline(ss, str, ',');
        Airline_temp.arriv_airport = stod(str);
        getline(ss, str, ',');
        Airline_temp.depart_time = str;
        getline(ss, str, ',');
        Airline_temp.arriv_time = str;
        getline(ss, str, ',');
        Airline_temp.airplane_id = stod(str);
        getline(ss, str, ',');
        Airline_temp.airplane_model = stod(str);
        getline(ss, str, ',');
        Airline_temp.fare = stod(str);

        //日期转为int类型
        Airline_temp.depart_date_i = (Airline_temp.departure_date[0] - 48) * 10 + (Airline_temp.departure_date[2] - 48);
        //出发、到达时间转为int类型
        Airline_temp.depart_time_i = turn_time_to_int(Airline_temp.depart_time);
        Airline_temp.arriv_time_i = turn_time_to_int(Airline_temp.arriv_time);

        /*
        //出发时间转为int类型
        if (Airline_temp.depart_time[10] == ':')
            Airline_temp.depart_time_i = (Airline_temp.depart_time[9] - 48) * 100 + (Airline_temp.depart_time[11] - 48) * 10 + (Airline_temp.depart_time[12] - 48);
        else if (Airline_temp.depart_time[11] == ':')
            Airline_temp.depart_time_i = (Airline_temp.depart_time[9] - 48) * 1000 + (Airline_temp.depart_time[10] - 48) * 100 + (Airline_temp.depart_time[12] - 48) * 10 + (Airline_temp.depart_time[13] - 48);
        Airline_temp.depart_time_i += (Airline_temp.depart_time[0] - 48) * 100000 + (Airline_temp.depart_time[2] - 48) * 10000;

        //到达时间转为int类型
        if (Airline_temp.arriv_time[10] == ':')
            Airline_temp.arriv_time_i = (Airline_temp.arriv_time[9] - 48) * 100 + (Airline_temp.arriv_time[11] - 48) * 10 + (Airline_temp.arriv_time[12] - 48);
        else if (Airline_temp.arriv_time[11] == ':')
            Airline_temp.arriv_time_i = (Airline_temp.arriv_time[9] - 48) * 1000 + (Airline_temp.arriv_time[10] - 48) * 100 + (Airline_temp.arriv_time[12] - 48) * 10 + (Airline_temp.arriv_time[13] - 48);
        Airline_temp.arriv_time_i += (Airline_temp.arriv_time[0] - 48) * 100000 + (Airline_temp.arriv_time[2] - 48) * 10000;
        */
        AirlineVector.push_back(Airline_temp);
    }
    //读入正确性检查
    /*
    for (int i = 1; i <= 9; i++)
    {
        cout << AirlineVector[i].airline_id << ' ' << AirlineVector[i].airplane_model << ' ' << AirlineVector[i].fare << ' ' << AirlineVector[i].arriv_time_i << endl;
    }*/

    return;
}

//创建邻接表
void CreateAL1(ALGraph *g)
{
    int end, start;
    g->maxVertexes = 79; //节点数
    g->numEdge = 2346;   //边数

    //每个顶点保存的数据
    for (int i = 0; i <= g->maxVertexes; i++)
    {
        g->adjList[i].airport_idd = i;  //第0个机场为1
        g->adjList[i].firstedge = NULL; //初始化时让每个顶点的指针域为NULL
    }

    //每条边的两个顶点
    for (int j = 1; j <= g->numEdge; j++)
    {
        // cout << "请输入第" << j << "条边:" << endl;
        // AirlineVector[j + 1].arriv_airport , AirlineVector[j + 1].depart_airport;

        EdgeNode *node = (EdgeNode *)malloc(sizeof(EdgeNode));
        start = AirlineVector[j].depart_airport;
        end = AirlineVector[j].arriv_airport;

        //如果有重复就不加入（去重）
        int flag = 1;
        EdgeNode *next = g->adjList[start].firstedge;
        while (next != NULL)
        {
            if (g->adjList[next->adjvex].airport_idd == end)
            {
                flag = 0;
                break;
            }
            next = next->next;
        }
        if (flag == 1)
        {
            node->adjvex = end;
            node->next = g->adjList[start].firstedge;
            g->adjList[start].firstedge = node;
        }
    }
}
//打印邻接表
void disp(ALGraph *g)
{
    cout << "邻接表为:" << endl;
    for (int i = 1; i <= g->maxVertexes; i++)
    {
        cout << g->adjList[i].airport_idd << ": ";
        PrintLink(g, g->adjList[i].firstedge);
    }
}
//打印链表
void PrintLink(ALGraph *g, EdgeNode *next)
{
    while (next != NULL)
    {
        cout << g->adjList[next->adjvex].airport_idd << " ";
        next = next->next;
    }
    cout << endl;
}
// task1

//对i节点的dfs遍历
vector<int> dfs_ans;
void DFS(ALGraph *G, int i)
{
    EdgeNode *p;
    visit_dfs[i] = 1;
    dfs_ans.push_back(G->adjList[i].airport_idd);
    // printf("%d ", G->adjList[i].airport_idd);
    p = G->adjList[i].firstedge;
    while (p)
    {
        if (!visit_dfs[p->adjvex])
        {
            DFS(G, p->adjvex);
        }
        p = p->next;
    }
}
void task1_dfs_find(ALGraph *G, int i)
{
    // cout << "task1_dfs_find:" << endl;
    DFS(G, i);
    int num = dfs_ans.size();
    int temp[90];
    for (int i = 0; i < num; i++)
        temp[i] = dfs_ans[i];
    sort(temp, temp + num);

    for (int i = 0; i < dfs_ans.size(); i++) //字典序
    {
        cout << temp[i] << " ";
    }
    cout << endl;
    /*
    for (int i = 0; i < dfs_ans.size(); i++)//非字典序
    {
        cout << dfs_ans[i] << " ";
    }
    */
    cout << endl;
    vector<int>().swap(dfs_ans);
}

// dfs遍历，用不上
void DFSTraverse(ALGraph *G)
{
    int i;
    for (i = 0; i < G->maxVertexes; i++)
    {
        visit_dfs[i] = 0;
    }
    for (i = 0; i < G->maxVertexes; i++)
    {
        if (!visit_dfs[i])
        {
            DFS(G, i);
        }
    }
}
/*
void task1_dfs_find(ALGraph *G, int i)
{
    BFS(G, i);
}


vector<int> bfs_ans;
void BFS(ALGraph *G, int i)
{
    EdgeNode *p;
    visit_bfs[i] = 1;
    dfs_ans.push_back(G->adjList[i].airport_idd);
    // printf("%d ", G->adjList[i].airport_idd);
    p = G->adjList[i].firstedge;
    while (p)
    {
        if (!visit_dfs[p->adjvex])
        {
            DFS(G, p->adjvex);
        }
        p = p->next;
    }

    EdgeNode *p;
    queue<EdgeNode *> Q;
    visit_bfs[i] = 1;
    bfs_ans.push_back(G->adjList[i].airport_idd);
    p = G->adjList[i].firstedge;

    Q.push(p);

    while (!Q.empty())
    {
        //队列不空
        EdgeNode e = Q.pop(); //获取出队列的那个VertexNode；

        for (auto i = e.firstedge; i; i = i->next)
        {
            // for循环链表 一直到null链表
            //其实是来找索引的
            if (!visit_dfs[i->adjvex])
            {
                Q.EnQueue(this->AdList[i->adjvex]); //判断索引没有被访问 进队列
                visit_dfs[i->adjvex] = true;
            }
        }
    }
}
*/

//打印vector，用于输出/debug
void print_vec(vector<int> &vec)
{
    for (auto elem : vec)
    {
        cout << elem << " ";
    }
    cout << endl
         << "---------------" << endl;
}

int main()
{
    cout << "————————————————————————————————————————————" << endl;
    cout << "********* 欢迎使用本航班查询系统 *********" << endl;
    ReadData();
    ALGraph *g1 = (ALGraph *)malloc(sizeof(ALGraph)); //创建邻接表
    CreateAL1(g1);                                    //屏幕打印邻接表
    // disp(g);
    /*AListGraph g2; //不要用malloc！！！
    // AListGraph *g2 = (AListGraph *)malloc(sizeof(AListGraph)); //创建邻接表
    CreateAL2(&g2); //屏幕打印邻接表
    dispAL2(&g2);*/
    AL3 g3; //不要用malloc！！！
    // AListGraph *g2 = (AListGraph *)malloc(sizeof(AListGraph)); //创建邻接表
    CreateAL3(&g3); //屏幕打印邻接表
    // cout << "finish creating" << endl;

    int mode; //查询模式
    cout << "请输入查询模式：" << endl;
    cin >> mode;
    // task1————————————————————
    if (mode == 1)
    {
        cout << "请输入需要查询的机场ID:" << endl;
        int task1_first_airport;
        cin >> task1_first_airport;
        cout << "机场" << task1_first_airport << "可到达的机场有：" << endl;
        task1_dfs_find(g1, task1_first_airport);
    }
    // task2————————————————————
    else if (mode == 2)
    {
        cout << "请输入中转次数：（-1代表无中转次数限制）" << endl;
        int trans_times;
        cin >> trans_times;

        int point[80];
        for (int i = 0; i <= VERTEX; i++)
            point[i] = i;
        // print_vec(point);
        MatrixGraph<int> matrixGraph(point, VERTEX, EDGE, 1); //邻接矩阵
        // matrixGraph.Output();
        cout << "-----------" << endl;
        if (trans_times == -1) //无限制搜索
        {
            for (int k = 1; k <= VERTEX; k++)
            {
                // cout << "dfs" << k << endl;
                matrixGraph.DFS_unlimited(k, k);
                for (int i = 0; i <= VERTEX; i++)
                    visited[i] = false;
                // cout << endl;
            }
            MatrixGraph<int> maxstep_matrixGraph(point, VERTEX, EDGE, 2); //根据最大步数规划的邻接矩阵
            maxstep_matrixGraph.Output();
        }
        else if (trans_times >= 0) //有中转次数限制搜索
        {
            for (int k = 1; k <= VERTEX; k++)
            {
                // cout << "dfs" << k << endl;
                matrixGraph.DFS(k, k, 0, trans_times + 1);
                for (int i = 0; i <= VERTEX; i++)
                    visited[i] = false;
                // cout << endl;
            }
            MatrixGraph<int> maxstep_matrixGraph(point, VERTEX, EDGE, 2); //根据最大步数规划的邻接矩阵
            maxstep_matrixGraph.Output();
            // cout << "bfs" << endl;
            // matrixGraph.BFS(1, 1);
        }
    }
    // task3————————————————————
    else if (mode == 3)
    {
        int airp1_id;
        int airp2_id;
        int trans_times;
        int airline_num;
        cout << "请分别输入两个机场的id和中转次数（0或1）：" << endl;
        cin >> airp1_id >> airp2_id >> trans_times;

        int point[80];
        for (int i = 0; i <= VERTEX; i++)
            point[i] = i;
        MatrixGraph<int> matrixGraph(point, VERTEX, EDGE, 1); //邻接矩阵

        if (trans_times == 0) //假如不需要中转，直接搜索得到航班数即可
        {
            // matrixGraph.Output();
            //  cout << "__________" << endl;
            airline_num = matrixGraph.get_arcij(airp1_id, airp2_id);
            cout << "满足要求的航线数目为：";
            cout << airline_num << endl;
        }

        else if (trans_times == 1)
        {
            task3_find_time_1trans_airline();

            airline_num = time_1trans_air[airp1_id][airp2_id];
            cout << "满足要求的航线数目为：";
            cout << airline_num << endl;
        }
        else
            cout << "illegal input" << endl;

        cout << "---------------------" << endl;
    }

    else if (mode == 4)
    {
        int ap1_id, ap2_id, min_time = 8888888;
        cout << "请输入两机场的ID:" << endl;
        cin >> ap1_id >> ap2_id;
        t4_init();

        t4_get_airline_from_ap(ap1_id); //以ap1_id出发的所有航线编号
        t4_get_airline_to_ap(ap2_id);   //以ap2_id到达的所有航线编号
        int cnt1 = air_id_from_ap.size();
        int cnt2 = air_id_to_ap.size();

        for (int i = 0; i < cnt1; i++) //对每一个从起点机场出发的航线
        {
            t4_init(); //每次dij之前都要初始化

            int al1 = air_id_from_ap[i];
            int time1 = AirlineVector[al1].depart_time_i;

            t4_dij(&g3, al1);

            for (int j = 0; j < cnt2; j++)
            {
                int al2 = air_id_to_ap[j];
                t = d[al2];
                if (t == inf)
                    continue;
                int time2 = t;
                int d_time = cal_during_time(time1, time2);
                if (d_time < min_time)
                    min_time = d_time;
            }
        }
        cout << "最短飞行时间为：" << min_time << "分钟" << endl;
    }

    else if (mode == 5)
    {
        int req, ap1_id, ap2_id, k_max;
        cout << "请输入您的要求代码：（1为起飞时段要求，2为降落时段要求，3为机型要求" << endl;
        cin >> req;
        if (req == 1)
        {
            cout << "请输入两机场的ID、中转次数上限k:" << endl;
            cin >> ap1_id >> ap2_id >> k_max;
            getchar();
            string max_t, min_t;

            cout << "请输入起飞时段上限:" << endl;
            getline(cin, max_t);
            // max_t = "5/8/2017 22:00";
            cout << "请输入起飞时段下限:" << endl;
            getline(cin, min_t);
            // min_t = "5/3/2017 23:00";

            //转为int类型的时间
            int max_t1 = turn_time_to_int(max_t);
            int min_t1 = turn_time_to_int(min_t);

            t5_init();

            t51_get_airline_from_ap(ap1_id, min_t1, max_t1); //以ap1_id出发的所有航线编号
            t51_get_airline_to_ap(ap2_id);                   //以ap2_id到达的所有航线编号
            int cnt1 = air_id_from_ap.size();
            int cnt2 = air_id_to_ap.size();

            if (cnt1 == 0 || cnt2 == 0) //一定不存在该线路
            {
                cout << "-1...很抱歉，找不到符合您要求的航班..." << endl;
            }
            else
            {
                int sum = 0;
                int temp_path[20000];
                int dest;
                for (int i = 0; i < cnt1; i++) //对每一个从起点机场出发的航线
                {
                    int al1 = air_id_from_ap[i];

                    t5_init(); //每次dij之前都要初始化
                    t5_dij(&g3, al1, k_max, 1, 0);
                    // t5_dij(&g3, al1, k_max, 2, 0);
                    // t5_dij(&g3, al1, k_max, 3, model);

                    for (int j = 0; j < cnt2; j++)
                    {
                        int al2 = air_id_to_ap[j];
                        // cout << "al2: " << al2 << endl;
                        int f = d5[al2];
                        // cout << "f: " << f << endl;
                        if (f == inf)
                            continue;

                        for (int i = 1; i <= 5000; i++)
                        {
                            temp_path[i] = path5[i];
                        }
                        cout << "航线：";
                        t5_print_path(temp_path, al2);
                        cout << endl;
                        sum++;
                    }
                }
                // cout << "---------------- 已为您找到费用最低的航线 --------------" << endl;
                cout << "已为您查询到" << sum << "条满足要求的航线..." << endl;
                // cout << "总费用为：" << min_fare << " 元" << endl;
            }
        }
        else if (req == 2) //降落时段要求
        {
            cout << "请输入两机场的ID、中转次数上限k:" << endl;
            cin >> ap1_id >> ap2_id >> k_max;
            getchar();
            string max_t, min_t;

            cout << "请输入降落时段上限:" << endl;
            getline(cin, max_t);
            // max_t = "5/9/2017 23:59";
            cout << "请输入降落时段下限:" << endl;
            getline(cin, min_t);
            // min_t = "5/2/2017 0:00";

            //转为int类型的时间
            int max_t1 = turn_time_to_int(max_t);
            int min_t1 = turn_time_to_int(min_t);

            t5_init();

            t52_get_airline_from_ap(ap1_id);               //以ap1_id出发的所有航线编号
            t52_get_airline_to_ap(ap2_id, min_t1, max_t1); //以ap2_id到达的所有航线编号

            int cnt1 = air_id_from_ap.size();
            int cnt2 = air_id_to_ap.size();

            // cout << cnt1 << " " << cnt2 << endl;
            if (cnt1 == 0 || cnt2 == 0) //一定不存在该线路
            {
                cout << "-1...很抱歉，找不到符合您要求的航班..." << endl;
            }
            else
            {
                int sum = 0;
                int temp_path[20000];
                for (int i = 0; i < cnt1; i++) //对每一个从起点机场出发的航线
                {
                    int al1 = air_id_from_ap[i];

                    t5_init(); //每次dij之前都要初始化
                    // void t5_dij(AL3 *g, int start, int k_max, int mode, int model)
                    t5_dij(&g3, al1, k_max, 2, 0);
                    // t5_dij(&g3, al1, k_max, 3, model);

                    for (int j = 0; j < cnt2; j++)
                    {
                        int al2 = air_id_to_ap[j];
                        // cout << "al2: " << al2 << endl;
                        int f = d5[al2];
                        // cout << "f: " << f << endl;
                        if (f == inf)
                            continue;

                        for (int i = 1; i <= 5000; i++)
                        {
                            temp_path[i] = path5[i];
                        }
                        cout << "航线：";
                        t5_print_path(temp_path, al2);
                        cout << endl;
                        sum++;
                    }
                }
                cout << "已为您查询到" << sum << "条满足要求的航线..." << endl;
            }
        }
        else if (req == 3) //机型有要求
        {
            cout << "请输入两机场的ID、中转次数上限:" << endl;
            cin >> ap1_id >> ap2_id >> k_max;
            int model;
            cout << "请输入机型的编号:" << endl;
            cin >> model;

            t5_init();

            t53_get_airline_from_ap(ap1_id, model); //以ap1_id出发的所有航线编号
            t53_get_airline_to_ap(ap2_id, model);   //以ap2_id到达的所有航线编号
            int cnt1 = air_id_from_ap.size();
            int cnt2 = air_id_to_ap.size();

            if (cnt1 == 0 || cnt2 == 0) //一定不存在该线路
            {
                cout << "-1...很抱歉，找不到符合您要求的航班..." << endl;
            }
            else
            {
                int sum = 0;
                int temp_path[20000];
                for (int i = 0; i < cnt1; i++) //对每一个从起点机场出发的航线
                {
                    int al1 = air_id_from_ap[i];

                    t5_init(); //每次dij之前都要初始化
                    // void t5_dij(AL3 *g, int start, int k_max, int mode, int model)
                    t5_dij(&g3, al1, k_max, 3, model);

                    for (int j = 0; j < cnt2; j++)
                    {
                        int al2 = air_id_to_ap[j];
                        // cout << "al2: " << al2 << endl;
                        int f = d5[al2];
                        // cout << "f: " << f << endl;
                        if (f == inf)
                            continue;

                        for (int i = 1; i <= 3000; i++)
                        {
                            temp_path[i] = path5[i];
                        }
                        cout << "航线：";
                        t5_print_path(temp_path, al2);
                        cout << endl;
                        sum++;
                    }
                }
                cout << "已为您查询到" << sum << "条满足要求的航线..." << endl;
            }
        }
        else
            cout << "illegal input" << endl;
        cout << "---------------------" << endl;
    }

    else if (mode == 6)
    {
        int req, min_fare = 8888888;
        int ap1_id, ap2_id;
        cout << "请输入您的要求代码:(1为起飞时段要求,2为降落时段要求,3为机型要求)" << endl;
        cin >> req;
        if (req == 1) //起飞时段有要求
        {
            cout << "请输入两机场的ID:" << endl;
            cin >> ap1_id >> ap2_id;
            getchar();
            string max_t, min_t;
            t6_init();

            cout << "请输入起飞时段上限:" << endl;
            getline(cin, max_t);
            // max_t = "5/9/2017 23:59";
            cout << "请输入起飞时段下限:" << endl;
            getline(cin, min_t);
            // min_t = "5/2/2017 0:00";

            //转为int类型的时间
            int max_t1 = turn_time_to_int(max_t);
            int min_t1 = turn_time_to_int(min_t);

            t51_get_airline_from_ap(ap1_id, min_t1, max_t1); //以ap1_id出发的所有航线编号
            t51_get_airline_to_ap(ap2_id);                   //以ap2_id到达的所有航线编号
            int cnt1 = air_id_from_ap.size();
            int cnt2 = air_id_to_ap.size();

            /*for (int i = 0; i < cnt1; i++)
                cout << air_id_from_ap[i] << " ";
            cout << endl;
            for (int i = 0; i < cnt2; i++)
                cout << air_id_to_ap[i] << " ";
            cout << endl;*/
            if (cnt1 == 0 || cnt2 == 0) //一定不存在该线路
            {
                cout << "-1...很抱歉，找不到符合您要求的航班..." << endl;
                return 0;
            }

            int temp_path[20000];
            int dest;
            int from;
            for (int i = 0; i < cnt1; i++) //对每一个从起点机场出发的航线
            {
                int al1 = air_id_from_ap[i];

                int flag = 0;

                t6_init(); //每次dij之前都要初始化
                t6_dij(&g3, al1, 1, 0);

                for (int j = 0; j < cnt2; j++)
                {
                    int al2 = air_id_to_ap[j];
                    // cout << "al2: " << al2 << endl;
                    int f = d6[al2];
                    // cout << "f: " << f << endl;
                    if (f == inf)
                        continue;
                    if (f < min_fare)
                    {
                        min_fare = f;
                        dest = al2;
                        flag = 1;
                        from = al1;
                    }
                }
                // cout << al1 << "->";
                if (flag == 1)
                {
                    for (int i = 1; i <= 5000; i++)
                    {
                        temp_path[i] = path6[i];
                    }
                }
            }
            // cout << from << "->";
            cout << "---------------- 已为您找到费用最低的航线 --------------" << endl;
            cout << "该航线为：";
            t6_print_path(temp_path, dest);
            cout << endl;
            cout << "总费用为：" << min_fare << " 元" << endl;
        }

        else if (req == 2) //降落时段有要求
        {
            cout << "请输入两机场的ID:" << endl;
            cin >> ap1_id >> ap2_id;
            getchar();
            string max_t, min_t;
            t6_init();

            cout << "请输入降落时段上限:" << endl;
            getline(cin, max_t);
            // max_t = "5/9/2017 23:59";
            cout << "请输入降落时段下限:" << endl;
            getline(cin, min_t);
            // min_t = "5/2/2017 0:00";

            //转为int类型的时间
            int max_t1 = turn_time_to_int(max_t);
            int min_t1 = turn_time_to_int(min_t);

            t52_get_airline_from_ap(ap1_id);               //以ap1_id出发的所有航线编号
            t52_get_airline_to_ap(ap2_id, min_t1, max_t1); //以ap2_id到达的所有航线编号
            int cnt1 = air_id_from_ap.size();
            int cnt2 = air_id_to_ap.size();

            if (cnt1 == 0 || cnt2 == 0) //一定不存在该线路
            {
                cout << "-1...很抱歉，找不到符合您要求的航班..." << endl;
                return 0;
            }

            int temp_path[20000];
            int dest, from;
            for (int i = 0; i < cnt1; i++) //对每一个从起点机场出发的航线
            {
                int al1 = air_id_from_ap[i];
                int flag = 0;
                t6_init(); //每次dij之前都要初始化
                t6_dij(&g3, al1, 2, 0);

                for (int j = 0; j < cnt2; j++)
                {
                    int al2 = air_id_to_ap[j];
                    // cout << "al2: " << al2 << endl;
                    int f = d6[al2];
                    // cout << "f: " << f << endl;
                    if (f == inf)
                        continue;
                    if (f < min_fare)
                    {
                        min_fare = f;
                        dest = al2;
                        flag = 1;
                        from = al1;
                    }
                }
                if (flag == 1)
                {
                    for (int i = 1; i <= 5000; i++)
                    {
                        temp_path[i] = path6[i];
                    }
                }
            }
            // cout << from << "->";
            cout << "---------------- 已为您找到费用最低的航线 --------------" << endl;
            cout << "该航线为：";
            t6_print_path(temp_path, dest);
            cout << endl;
            cout << "总费用为：" << min_fare << " 元" << endl;
        }

        else if (req == 3) //机型有要求
        {
            cout << "请输入两机场的ID:" << endl;
            cin >> ap1_id >> ap2_id;
            int model;
            cout << "请输入机型的编号:" << endl;
            cin >> model;

            t53_get_airline_from_ap(ap1_id, model); //以ap1_id出发的所有航线编号
            t53_get_airline_to_ap(ap2_id, model);   //以ap2_id到达的所有航线编号
            int cnt1 = air_id_from_ap.size();
            int cnt2 = air_id_to_ap.size();

            if (cnt1 == 0 || cnt2 == 0) //一定不存在该线路
            {
                cout << "-1...很抱歉，找不到符合您要求的航班..." << endl;
                return 0;
            }

            int temp_path[20000];
            int dest, from;
            for (int i = 0; i < cnt1; i++) //对每一个从起点机场出发的航线
            {
                int al1 = air_id_from_ap[i];
                int flag = 0;
                t6_init(); //每次dij之前都要初始化
                t6_dij(&g3, al1, 3, model);

                for (int j = 0; j < cnt2; j++)
                {
                    int al2 = air_id_to_ap[j];
                    // cout << "al2: " << al2 << endl;
                    int f = d6[al2];
                    // cout << "f: " << f << endl;
                    if (f == inf)
                        continue;
                    if (f < min_fare)
                    {
                        min_fare = f;
                        dest = al2;
                        flag = 1;
                        from = al1;
                    }
                }
                if (flag == 1)
                {
                    for (int i = 1; i <= 5000; i++)
                    {
                        temp_path[i] = path6[i];
                    }
                }
            }
            // cout << from << "->";
            cout << "---------------- 已为您找到费用最低的航线 --------------" << endl;
            cout << "该航线为：";
            t6_print_path(temp_path, dest);
            cout << endl;
            cout << "总费用为：" << min_fare << " 元" << endl;
        }
        else
            cout << "illegal input" << endl;
        cout << "---------------------" << endl;
    }

    else if (mode == 7)
    {
        int req, ap1_id, ap2_id, k_max, max_trans_t_single, max_trans_t_sum;

        cout << "请输入两机场的ID、中转次数上限k:" << endl;
        cin >> ap1_id >> ap2_id >> k_max;
        cout << "请输入您的要求:(1为单次中转时间上限,2为总中转时间上限):" << endl;
        cin >> req;
        if (req == 1) //单次中转时间上限
        {
            cout << "请输入单次中转时间上限:" << endl;
            cin >> max_trans_t_single;

            t4_get_airline_from_ap(ap1_id); //以ap1_id出发的所有航线编号
            t4_get_airline_to_ap(ap2_id);   //以ap2_id到达的所有航线编号
            int cnt1 = air_id_from_ap.size();
            int cnt2 = air_id_to_ap.size();

            t7_init();
            if (cnt1 == 0 || cnt2 == 0) //一定不存在该线路
            {
                cout << "-1...很抱歉，找不到符合您要求的航班..." << endl;
            }
            else
            {
                int sum = 0;
                int temp_path[20000];
                for (int i = 0; i < cnt1; i++) //对每一个从起点机场出发的航线
                {
                    int al1 = air_id_from_ap[i];

                    t7_init(); //每次dij之前都要初始化
                    t7_dij(&g3, al1, k_max, req, max_trans_t_single);

                    for (int j = 0; j < cnt2; j++)
                    {
                        int al2 = air_id_to_ap[j];
                        // cout << "al2: " << al2 << endl;
                        int f = d7[al2];
                        // cout << "f: " << f << endl;
                        if (f == inf)
                            continue;

                        for (int i = 1; i <= 3000; i++)
                        {
                            temp_path[i] = path7[i];
                        }
                        cout << "航线：";
                        t7_print_path(temp_path, al2);
                        cout << endl;
                        sum++;
                    }
                }
                cout << "已为您查询到" << sum << "条满足要求的航线..." << endl;
            }
        }
        else if (req == 2) //总中转时间上限
        {
            cout << "请输入总中转时间上限:" << endl;
            cin >> max_trans_t_sum;

            t4_get_airline_from_ap(ap1_id); //以ap1_id出发的所有航线编号
            t4_get_airline_to_ap(ap2_id);   //以ap2_id到达的所有航线编号
            int cnt1 = air_id_from_ap.size();
            int cnt2 = air_id_to_ap.size();

            t7_init();
            if (cnt1 == 0 || cnt2 == 0) //一定不存在该线路
            {
                cout << "-1...很抱歉，找不到符合您要求的航班..." << endl;
            }
            else
            {
                int sum = 0;
                int temp_path[20000];
                for (int i = 0; i < cnt1; i++) //对每一个从起点机场出发的航线
                {
                    int al1 = air_id_from_ap[i];

                    t7_init(); //每次dij之前都要初始化
                    t7_dij(&g3, al1, k_max, req, max_trans_t_sum);

                    for (int j = 0; j < cnt2; j++)
                    {
                        int al2 = air_id_to_ap[j];
                        // cout << "al2: " << al2 << endl;
                        int f = d7[al2];
                        // cout << "f: " << f << endl;
                        if (f == inf)
                            continue;

                        for (int i = 1; i <= 3000; i++)
                        {
                            temp_path[i] = path7[i];
                        }
                        cout << "航线：";
                        t7_print_path(temp_path, al2);
                        cout << endl;
                        sum++;
                    }
                }
                cout << "已为您查询到" << sum << "条满足要求的航线..." << endl;
            }
        }
    }

    else if (mode == 8)
    {
        int req, min_fare = 8888888;
        int ap1_id, ap2_id, max_trans_t_single, max_trans_t_sum;
        cout << "请输入两机场的ID:" << endl;
        cin >> ap1_id >> ap2_id;
        cout << "请输入您的要求:(1为单次中转时间上限,2为总中转时间上限):" << endl;
        cin >> req;

        if (req == 1) //单次中转时间上限
        {
            cout << "请输入单次中转时间上限:" << endl;
            cin >> max_trans_t_single;

            t4_get_airline_from_ap(ap1_id); //以ap1_id出发的所有航线编号
            t4_get_airline_to_ap(ap2_id);   //以ap2_id到达的所有航线编号
            int cnt1 = air_id_from_ap.size();
            int cnt2 = air_id_to_ap.size();

            if (cnt1 == 0 || cnt2 == 0) //一定不存在该线路
            {
                cout << "-1...很抱歉，找不到符合您要求的航班..." << endl;
                return 0;
            }

            int temp_path[20000];
            int dest, from;
            for (int i = 0; i < cnt1; i++) //对每一个从起点机场出发的航线
            {
                int al1 = air_id_from_ap[i];
                int flag = 0;
                t8_init(); //每次dij之前都要初始化
                t8_dij(&g3, al1, req, max_trans_t_single);

                for (int j = 0; j < cnt2; j++)
                {
                    int al2 = air_id_to_ap[j];
                    // cout << "al2: " << al2 << endl;
                    int f = d8[al2];
                    // cout << "f: " << f << endl;
                    if (f == inf)
                        continue;
                    if (f < min_fare)
                    {
                        min_fare = f;
                        dest = al2;
                        flag = 1;
                        from = al1;
                    }
                }
                if (flag == 1)
                {
                    for (int i = 1; i <= 5000; i++)
                    {
                        temp_path[i] = path8[i];
                    }
                }
            }
            cout << "---------------- 已为您找到费用最低的航线 --------------" << endl;
            cout << "该航线为：";
            t8_print_path(temp_path, dest);
            cout << endl;
            cout << "总费用为：" << min_fare << " 元" << endl;
        }
        else if (req == 2) //总中转时间上限
        {
            cout << "请输入总中转时间上限:" << endl;
            cin >> max_trans_t_sum;

            t4_get_airline_from_ap(ap1_id); //以ap1_id出发的所有航线编号
            t4_get_airline_to_ap(ap2_id);   //以ap2_id到达的所有航线编号
            int cnt1 = air_id_from_ap.size();
            int cnt2 = air_id_to_ap.size();

            if (cnt1 == 0 || cnt2 == 0) //一定不存在该线路
            {
                cout << "-1...很抱歉，找不到符合您要求的航班..." << endl;
                return 0;
            }

            int temp_path[20000];
            int dest, from;
            for (int i = 0; i < cnt1; i++) //对每一个从起点机场出发的航线
            {
                int al1 = air_id_from_ap[i];
                int flag = 0;
                t8_init(); //每次dij之前都要初始化
                t8_dij(&g3, al1, 2, max_trans_t_sum);

                for (int j = 0; j < cnt2; j++)
                {
                    int al2 = air_id_to_ap[j];
                    // cout << "al2: " << al2 << endl;
                    int f = d8[al2];
                    // cout << "f: " << f << endl;
                    if (f == inf)
                        continue;
                    if (f < min_fare)
                    {
                        min_fare = f;
                        dest = al2;
                        flag = 1;
                        from = al1;
                    }
                }
                if (flag == 1)
                {
                    for (int i = 1; i <= 5000; i++)
                    {
                        temp_path[i] = path8[i];
                    }
                }
            }
            cout << "---------------- 已为您找到费用最低的航线 --------------" << endl;
            cout << "该航线为：";
            t8_print_path(temp_path, dest);
            cout << endl;
            cout << "总费用为：" << min_fare << " 元" << endl;
        }
    }

    cout << "******* 欢迎您再次使用本航班查询系统 *******";
    return 0;
}
