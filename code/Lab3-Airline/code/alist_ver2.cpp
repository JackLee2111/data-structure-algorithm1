// task4-6 邻接表————————————————————————————————————————————————————————
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

// task4-6 建立邻接表
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

_________________________________________________________________
AListGraph g2;
// AListGraph *g2 = (AListGraph *)malloc(sizeof(AListGraph)); //创建邻接表
CreateAL2(&g2); //屏幕打印邻接表
dispAL2(&g2);

// task4-6 邻接表————————————————————————————————————————————————————————
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
    list<ENode> Node;
} AList;
//图
typedef struct
{
    AList aList[VERTEX_NUM];
    int Vertex, Edge;
} AListGraph;
// task4-6 建立邻接表
void CreateAL2(AListGraph *g)
{
    cout << "CreateAL2" << endl;
    int end, start;
    g->Vertex = 79; //节点数
    g->Edge = 2346; //边数

    //每个顶点保存的数据
    for (int i = 0; i <= 79; i++)
    {
        g->aList[i].airport_id = i; //第0个机场为1
    }
    cout << "finish 每个顶点保存的数据" << endl;

    //每条边的两个顶点
    for (int i = 1; i <= 2346; i++)
    {
        cout << "insert num " << i << endl;
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
        cout << "finish get data" << endl;

        g->aList[start].Node.push_back(temp); //加入以start为起点的邻接表中v
        cout << "finish insert here" << endl;
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
    cout << "PrintLinkAL2" << endl;
    for (auto e : node.Node)
    {
        cout << e.airport_id << " ";
    }
    cout << endl;
}