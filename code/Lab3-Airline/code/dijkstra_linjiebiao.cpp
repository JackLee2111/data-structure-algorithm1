#include <stdio.h>
#include <stdlib.h>
#define MAXVEX 100
#define maxSize 100
#define INF 99999

typedef struct ArcNode
{
    int weight; //记录权值
    int adjvex; //邻接点
    struct ArcNode *nextarc;
} ArcNode;
typedef struct VNode
{
    int data;
    ArcNode *firstarc;
} VNode;
typedef struct AGraph
{
    VNode adjlist[MAXVEX];
    int numNodes, numEdges;
} AGraph;

//创建图
void CreateGraph(AGraph *G)
{
    int i;
    int m, n, weight;

    // FILE *fp = fopen("data.txt", "r");
    scanf("%d %d", &G->numNodes, &G->numEdges);

    for (i = 0; i < G->numNodes; i++)
        G->adjlist[i].firstarc = NULL;

    for (i = 0; i < G->numEdges; i++)
    {
        scanf("%d %d %d", &m, &n, &weight);
        ArcNode *pe = (ArcNode *)malloc(sizeof(ArcNode));
        pe->adjvex = n;
        pe->weight = weight;
        pe->nextarc = G->adjlist[m].firstarc;
        G->adjlist[m].firstarc = pe;

        // ArcNode *p = (ArcNode *)malloc(sizeof(ArcNode));
        // p->adjvex = n;
        // p->weight = weight;
        // p->nextarc = G->adjlist[m].firstarc;
        // G->adjlist[m].firstarc = p;
    }

    // fclose(fp);
}

//获得边的权重
float getWeight(AGraph *G, int u, int j)
{
    ArcNode *p = G->adjlist[u].firstarc;
    while (p != NULL)
    {
        if (p->adjvex == j)
            return p->weight;
        p = p->nextarc;
    }
    return INF;
}

//迪杰斯特拉算法
void Dijkstra(AGraph *G, int dist[], int path[], int v)
{
    int set[maxSize];
    int i, j, u, min;
    float weight;
    //给三个数组赋初值
    for (i = 0; i < G->numNodes; i++)
    {
        set[i] = 0;
        path[i] = -1;
        dist[i] = INF;
    }

    ArcNode *p = G->adjlist[v].firstarc;
    while (p != NULL)
    {
        dist[p->adjvex] = p->weight;
        path[p->adjvex] = v;
        p = p->nextarc;
    }
    path[v] = -1;
    set[v] = 1;
    dist[v] = 0;

    for (i = 0; i < G->numNodes - 1; i++)
    {
        min = INF;

        for (j = 0; j < G->numNodes; j++)
        {
            if (set[j] == 0 && dist[j] < min)
            {
                min = dist[j];
                u = j;
            }
        }
        set[u] = 1;

        for (j = 0; j < G->numNodes; j++)
        {
            float weight = getWeight(G, u, j);
            if (set[j] == 0 && dist[u] + weight < dist[j])
            {
                dist[j] = dist[u] + weight;
                path[j] = u;
            }
        }
    }
}

//输出路径
void print_path(int path[], int v1)
{
    if (path[v1] == -1)
        printf("%d ", v1);
    else
    {
        print_path(path, path[v1]);
        printf("%d ", v1);
    }
}

int main()
{
    AGraph *G = (AGraph *)malloc(sizeof(AGraph));
    int v, v1;

    CreateGraph(G);

    int *path = (int *)malloc(sizeof(int) * G->numNodes);
    int *dist = (int *)malloc(sizeof(int) * G->numNodes);

    printf("输入起始点: ");
    scanf("%d", &v);

    printf("输入终止点: ");
    scanf("%d", &v1);

    Dijkstra(G, dist, path, v);

    printf("最短路径为: ");
    print_path(path, v1);

    getchar();
    getchar();
    return 0;
}
