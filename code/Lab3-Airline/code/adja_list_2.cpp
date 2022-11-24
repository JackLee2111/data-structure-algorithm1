#include <stdio.h>
#include <iostream>
#include <malloc.h>
#define maxSize 1000
using namespace std;
typedef struct ArcNode
{
    int adjvex; //该边所指向的节点的位置
    struct ArcNode *nextarc;
    int info; //边的信息, 例如边的权重
} ArcNode;

typedef struct
{
    int data;          //顶点表存储当前顶点的信息
    ArcNode *firstarc; //指向边的指针
} Vnode;

typedef struct //邻接表的图
{
    Vnode adjlist[maxSize]; //顶点数组
    int n;                  //顶点数
    int e;                  //边数
} Adja_list_Graph;

Adja_list_Graph *graph;
void insertNode(ArcNode *node, ArcNode *newNode)
{
    ArcNode *p = node;
    while (p->nextarc != NULL)
    {
        p = p->nextarc;
    }
    p->nextarc = newNode;
}

void create()
{
    graph = (Adja_list_Graph *)malloc(sizeof(Adja_list_Graph));
    cout << "输入顶点的数目: " << endl;
    cin >> graph->n;
    cout << "输入图中边的数目: " << endl;
    cin >> graph->e;
    cout << graph->n << " ";

    int u = -1, v = -1, weight = -1;
    for (int i = 0; i < graph->n; i++)
    {
        graph->adjlist[i].firstarc = NULL;
    }

    ArcNode *node;
    cout << graph->e << endl;
    for (int i = 0; i < graph->e; i++)
    {
        cin >> u >> v >> weight;
        node = (ArcNode *)malloc(sizeof(ArcNode));
        node->adjvex = v;
        node->info = weight;
        node->nextarc = NULL;
        graph->adjlist[u].data = u;
        if (graph->adjlist[u].firstarc == NULL)
        {
            //边
            graph->adjlist[u].firstarc = node;
        }
        else
        {
            //插入边
            insertNode(graph->adjlist[u].firstarc, node);
        }
    }
}

void travseTree()
{
    for (int i = 0; i < graph->n; i++)
    {
        if (graph->adjlist[i].firstarc != NULL)
        {
            cout << i << " ";
            ArcNode *p = graph->adjlist[i].firstarc;
            while (p != NULL)
            {
                cout << p->adjvex << " ";
                p = p->nextarc;
            }
            cout << endl;
        }
    }
}

int main(void)
{
    create();
    travseTree();
    return 0;
}