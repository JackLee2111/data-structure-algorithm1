#include <iostream>
#include <string.h>
struct OtherInfo
{
    char airport_idd;
    int iWeight;
};
struct EdgeNode
{
    OtherInfo other; //存储其他信息
    int adjvex;      //用于储存下表
    EdgeNode *next;  //链表形式 指向下一个ArcNode
};
struct VertexNode
{
    int airport_idd;
    EdgeNode *firstedge; //指向第一个
};
struct ALGraph
{
    int Vexnum, Acrnum;     //存储顶点数量
    VertexNode AdList[100]; //存储的是那个链表总共的
    ALGraph(int, int);
    void BFS(); // BFS算法
};
struct Queue
{
    VertexNode base[100]; //队列储存的元素
    int rear, front;
    Queue()
    {
        rear = front = 0; //初始化
    }
    void EnQueue(VertexNode EdgeNode);
    VertexNode DeQueue();
    bool empty();
};
bool Queue::empty()
{
    if (front == rear)
        return true;
    else
        return false;
}
void Queue::EnQueue(VertexNode node)
{
    if ((rear + 1) % 100 == front)
        return; //此时已经满了
    base[rear] = node;
    rear = (rear + 1) % 100;
}
VertexNode Queue::DeQueue()
{
    if (rear == front)
        return base[rear]; //此时已经是空了
    return base[front++];
}
int LocateVex(ALGraph G, char airport_idd)
{
    //这个Locate只能定位头顶点
    for (int i = 0; i < G.Vexnum; i++)
    {
        if (airport_idd == G.AdList[i].airport_idd)
            return i;
    }
}
ALGraph::ALGraph(int Vexnum, int Acrnum)
{
    this->Acrnum = Acrnum;
    this->Vexnum = Vexnum;
    for (int i = 0; i < Vexnum; i++)
    {
        //先初始化那个ALGraph VertexNode数组里面的名字 hezhizhen
        std::cout << "输入顶点名字:";
        std::cin >> this->AdList[i].airport_idd;
        this->AdList[i].firstedge = NULL; //把指针置空
    }
    for (int k = 0; k < Acrnum; k++)
    {
        //假如是无向网
        std::cout << "请输入顶点1 顶点2 和权重:";
        char cairport_idd1;
        char cairport_idd2;
        int iWeight;
        std::cin >> cairport_idd1 >> cairport_idd2 >> iWeight;
        auto i = LocateVex(*this, cairport_idd1);
        auto j = LocateVex(*this, cairport_idd2); //注意 这个时候返回的是总表 每个分头顶点的下表
        //从顶点一出发 作为头结点
        auto p = new EdgeNode; //指向链表的结点
        p->other.airport_idd = cairport_idd2;
        p->adjvex = j; //??存疑
        p->other.iWeight = iWeight;
        p->next = this->AdList[i].firstedge;
        this->AdList[i].firstedge = p; //运用头插法
        // 无向网 反过来
        auto p1 = new EdgeNode; //指向链表的结点
        p1->other.airport_idd = cairport_idd1;
        p1->adjvex = i; //??存疑
        p1->other.iWeight = iWeight;
        p1->next = this->AdList[j].firstedge;
        this->AdList[j].firstedge = p1; //运用头插法
    }
}
bool visited[5];

void ALGraph::BFS()
{
    Queue Q;
    std::cout << "请输入开始顶点的名字:";
    char airport_idd;
    std::cin >> airport_idd;
    Q.EnQueue(this->AdList[LocateVex(*this, airport_idd)]); //从这个索引开始
    visited[LocateVex(*this, airport_idd)] = true;          //从输入的那个顶点开始
    while (!Q.empty())
    {
        //队列不空
        auto e = Q.DeQueue(); //获取出队列的那个VertexNode；
        std::cout << e.airport_idd << "被访问";

        //
        for (auto i = e.firstedge; i; i = i->next)
        {
            // for循环链表 一直到null链表
            //其实是来找索引的
            if (!visited[i->adjvex])
            {
                Q.EnQueue(this->AdList[i->adjvex]); //判断索引没有被访问 进队列
                visited[i->adjvex] = true;
            }
        }
    }
}
int main()
{
    memset(visited, false, sizeof(visited));
    ALGraph G(5, 6);
    G.BFS();
    return 0;
}