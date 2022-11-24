/* 使用邻接矩阵储存图  */
#include <iostream>
#include <queue>
using namespace std;
bool visited[80] = {0};
const int MaxSize = 80; //最大顶点数

template <class DataType>
class MatrixGraph
{
public:
    MatrixGraph(DataType a[], int n, int e);
    void Output();
    void DFS(int v);
    void BFS(int v);

private:
    DataType vertex[MaxSize];  //顶点数组
    int arc[MaxSize][MaxSize]; //邻接矩阵
    int vertexNum, edgeNum;    //图中当前的顶点数与边数
};

/** 构造函数
 *@param n 顶点数
 *@param e 边数
 */
template <class DataType>
MatrixGraph<DataType>::MatrixGraph(DataType a[], int n, int e)
{
    int i, j, k;
    vertexNum = n;
    edgeNum = e;
    for (i = 0; i < vertexNum; i++)
        vertex[i] = a[i];           //顶点数组赋值
    for (i = 0; i < vertexNum; i++) //初始化邻接矩阵
        for (j = 0; j < vertexNum; j++)
            arc[i][j] = 0;
    for (k = 0; k < edgeNum; k++) //依次输入每一条边
    {
        cin >> i >> j; //输入边依附的两个顶点的编号
        arc[i][j] = 1;
        // arc[j][i] = 1; //无向图
    }
}

//输出邻接矩阵函数
template <class DataType>
void MatrixGraph<DataType>::Output()
{
    int i, j;
    for (i = 0; i < vertexNum; i++)
    { //输出邻接矩阵
        for (j = 0; j < vertexNum; j++)
        {
            cout << arc[i][j] << " ";
        }
        cout << endl;
    }
}

template <class DataType>
void MatrixGraph<DataType>::DFS(int v)
{
    cout << vertex[v] << " "; //输出顶点的值
    visited[v] = true;        //标记被访问
    for (int j = 0; j < vertexNum; j++)
    { // j 小于顶点数时
        if (arc[v][j] == 1 && visited[j] == false)
        {
            DFS(j);
        }
    }
}

template <class DataType>
void MatrixGraph<DataType>::BFS(int v) //传入顶点序号
{
    bool visited[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    queue<int> Q;             //初始化顺序队列
    cout << vertex[v] << " "; //输出顶点
    visited[v] = true;        //标记被访问
    Q.push(v);                //顶点v入队列
    while (!Q.empty())        //当队列非空时
    {
        v = Q.front(); //将队头元素取出送到v中
        Q.pop();
        for (int j = 0; j < vertexNum; j++)
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

int main()
{
    int a[5] = {0, 1, 2, 3}; //顶点数组V0,V1,V2,V3
    cout << "输入边（i,j）上的顶点" << endl;
    MatrixGraph<int> MatrixGraph(a, 4, 4); //传入顶点数组a, 顶点的个数以及边的个数
    MatrixGraph.Output();
    cout << endl
         << "DFS遍历结果：" << endl;
    MatrixGraph.DFS(0);
    cout << endl
         << "BFS遍历结果：";
    MatrixGraph.BFS(0);

    return 0;
}
