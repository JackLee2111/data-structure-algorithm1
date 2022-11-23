// AdjacencyList.cpp: 定义控制台应用程序的入口点。
#include <iostream>
#include <stdlib.h> //malloc函数在stdlib库中
using namespace std;

#define MAX_VERTEX_NUM 20
#define VertexType int

//边表结点
typedef struct ArcNode
{
    int Adjvex; //注意:这个索引值是边表节点自己在数组中的索引值，而不是和它连接顶点的下标。
    ArcNode *nextarc;
    int weight; //如果是有权图的话加这条语句，没有就不用加
} ArcNode;

//顶点表结点
typedef struct VNode
{
    VertexType data;
    ArcNode *firstarc;
} AdjList[MAX_VERTEX_NUM];

typedef struct
{
    AdjList vertices;
    int vexnum, arcnum;
} ALGraph;

void PrintLink(ALGraph *g, ArcNode *next);
void CreateAL(ALGraph *g);
void disp(ALGraph *g);

int main(void)
{
    ALGraph *g = (ALGraph *)malloc(sizeof(ALGraph)); //创建邻接表
    CreateAL(g);                                     //屏幕打印邻接表
    disp(g);
    system("pause");
    return 0;
}
/*创建邻接表函数，参数为一个邻接表变量
该函数将提示用户输入创建一个邻接表所需要的关键信息，
包括图中节点个数，边的个数，每个节点存储的数据，每条边两边节点的索引值*/
void CreateAL(ALGraph *g)
{
    int end, start;
    cout << "请输入结点数和边数：";
    cin >> g->vexnum >> g->arcnum;
    cout << "请输入每个顶点保存的数据：" << endl;
    for (int i = 0; i < g->vexnum; i++)
    {
        cout << "vertex" << i << ":";
        cin >> g->vertices[i].data;
        g->vertices[i].firstarc = NULL; //初始化时一定要让每个顶点的指针域为NULL，否则不会默认为NULL
    }
    cout << "请输入每条边的两个顶点在数组中的下标(中间用空格分隔)" << endl;

    for (int j = 0; j < g->arcnum; j++)
    {
        cout << "请输入第" << j << "条边:" << endl;
        cin >> start >> end;
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
        cout << g->vertices[i].data << " ";
        PrintLink(g, g->vertices[i].firstarc);
    }
}
/*打印链表函数，接收一个邻接表指针变量，和边表节点指针变量
该函数专门用于打印邻接表中链表的节点数据*/
void PrintLink(ALGraph *g, ArcNode *next)
{
    while (next != NULL)
    {
        cout << g->vertices[next->Adjvex].data << " ";
        next = next->nextarc;
    }
    cout << endl;
}
