#include <iostream>
#include <stdlib.h>
using namespace std;

#define MAX_VERTEX_NUM 20
#define VertexType int

//边表结点
typedef struct ArcNode
{
    int Adjvex;       //索引值是边表节点自己在数组中的索引值，而不是和它连接顶点的下标。
    ArcNode *nextarc; //下一条边指向的顶点
    int weight;       //有权图
} ArcNode;

//顶点表结点
typedef struct VNode
{
    VertexType airport_num;
    ArcNode *firstarc; //第一条边指向的顶点
} AdjList[MAX_VERTEX_NUM];

//图
typedef struct
{
    AdjList vertices;
    int vexnum, arcnum;
} ALGraph;

void PrintLink(ALGraph *g, ArcNode *next);
void CreateAL1(ALGraph *g);
void disp(ALGraph *g);

int main(void)
{
    ALGraph *g = (ALGraph *)malloc(sizeof(ALGraph)); //创建邻接表
    CreateAL1(g);                                    //屏幕打印邻接表
    disp(g);
    return 0;
}
/*创建邻接表函数，参数为一个邻接表变量
该函数将提示用户输入创建一个邻接表所需要的关键信息，
包括图中节点个数，边的个数，每个节点存储的数据，每条边两边节点的索引值*/
void CreateAL1(ALGraph *g)
{
    int end, start;
    g->vexnum = 79;   //节点数
    g->arcnum = 2346; //边数

    //每个顶点保存的数据
    for (int i = 0; i < g->vexnum; i++)
    {
        g->vertices[i].airport_num = i + 1;
        g->vertices[i].firstarc = NULL; //初始化时让每个顶点的指针域为NULL
    }

    //每条边的两个顶点
    for (int j = 0; j < g->arcnum; j++)
    {
        // cout << "请输入第" << j << "条边:" << endl;
        // cin >> start >> end;
        ArcNode *node = (ArcNode *)malloc(sizeof(ArcNode));
        node->Adjvex = end;
        node->nextarc = g->vertices[start].firstarc;
        g->vertices[start].firstarc = node;
    }
}
/*打印创建好的邻接表函数，接收一个邻接表指针变量
该函数将打印每个顶点以及与其相连的顶点中保存的数据*/
void disp(ALGraph *g)
{
    cout << "邻接表为:" << endl;
    for (int i = 0; i < g->vexnum; i++)
    {
        cout << g->vertices[i].airport_num << " ";
        PrintLink(g, g->vertices[i].firstarc);
    }
}
/*打印链表函数，接收一个邻接表指针变量，和边表节点指针变量
该函数专门用于打印邻接表中链表的节点数据*/
void PrintLink(ALGraph *g, ArcNode *next)
{
    while (next != NULL)
    {
        cout << g->vertices[next->Adjvex].airport_num << " ";
        next = next->nextarc;
    }
    cout << endl;
}
