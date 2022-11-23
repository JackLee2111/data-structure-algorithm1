#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

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
    int depart_time_i; //->int,例如 1220
    string arriv_time;
    int arriv_time_i; //->int,例如 1220

    int airplane_id;
    int airplane_model; //机型
    int fare;
};
Airline_data Airline_temp0;

vector<struct Airline_data> AirlineVector; //航班数据存储

// task1邻接表
vector<vector<pair<int, int>>> Adja_List_fare; // 邻接表{from, {to, fare}} （即pair<int, int> = {to, fare}）
void construct_Adja_List(vector<vector<int>> &times, int n);
void print_Adja_List_fare();
void Readdata_in_list_fare();

// task2邻接矩阵
bool visited[80] = {0}; // task2邻接矩阵
const int MaxSize = 80; // task2邻接矩阵最大顶点数

//优先队列cmp规则
struct cmp
{
    bool operator()(pair<int, int> &a, pair<int, int> &b)
    {
        return a.second < b.second;
    }
};

//打印vector
void print_vec(vector<int> &vec)
{
    for (auto elem : vec)
    {
        cout << elem << " ";
    }
    cout << endl
         << "---------------" << endl;
}

//打印邻接表的结构（目前没用）
void print_Adja_List_fare()
{
    int n = Adja_List_fare.size();
    for (int i = 0; i < n; ++i)
    {
        cout << i << ": ";
        for (auto elem : Adja_List_fare[i])
        {
            cout << "{" << elem.first << ", " << elem.second << "}, ";
        }
        cout << endl;
    }
}

// 邻接表构建图
void construct_Adja_List(vector<vector<int>> &times, int n)
{
    for (int i = 0; i < n; ++i)
    {
        Adja_List_fare.push_back(vector<pair<int, int>>());
    }
    for (auto edg : times)
    {
        Adja_List_fare[edg[0]].push_back({edg[1], edg[2]});
    }
    //打印邻接表构建的图
    // print_Adja_List();
}

//将数据存入vector，后续邻接表建图（目前没用）
vector<vector<int>> times1;
void Readdata_in_list_fare()
{
    int num = AirlineVector.size();
    for (int i = 1; i < num; i++)
    {
        vector<int> temp;
        temp.push_back(AirlineVector[i].depart_airport);
        temp.push_back(AirlineVector[i].arriv_airport);
        temp.push_back(AirlineVector[i].fare);
        // print_vec(temp);
        times1.push_back(temp);
        vector<int>().swap(temp);
    }
    int n = 79; //节点数（机场总数）

    construct_Adja_List(times1, n); //建图

    print_Adja_List_fare();

    return;
}

//数据读入
void ReadData()
{
    /*读入data.CSV文件*/
    ifstream infile("G:/2022 autumn semester/2022-data_structure_and_algorithm/code/Lab3-Airline/data.csv", ios::in);
    string line;

    getline(infile, line);
    AirlineVector.push_back(Airline_temp0); //塞一个空东西进去
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
        getline(ss, str, ',');
        Airline_temp.airplane_id = stod(str);
        getline(ss, str, ',');
        Airline_temp.airplane_model = stod(str);
        getline(ss, str, ',');
        Airline_temp.fare = stod(str);

        //日期转为int类型
        Airline_temp.depart_date_i = (Airline_temp.departure_date[0] - 48) * 10 + (Airline_temp.departure_date[2] - 48);
        //出发时间转为int类型
        if (Airline_temp.depart_time[10] == ':')
            Airline_temp.depart_time_i = (Airline_temp.depart_time[9] - 48) * 100 + (Airline_temp.depart_time[11] - 48) * 10 + (Airline_temp.depart_time[12] - 48);
        else if (Airline_temp.depart_time[11] == ':')
            Airline_temp.depart_time_i = (Airline_temp.depart_time[9] - 48) * 1000 + (Airline_temp.depart_time[10] - 48) * 100 + (Airline_temp.depart_time[12] - 48) * 10 + (Airline_temp.depart_time[13] - 48);
        //到达时间转为int类型
        if (Airline_temp.arriv_time[10] == ':')
            Airline_temp.arriv_time_i = (Airline_temp.arriv_time[9] - 48) * 100 + (Airline_temp.arriv_time[11] - 48) * 10 + (Airline_temp.arriv_time[12] - 48);
        else if (Airline_temp.depart_time[11] == ':')
            Airline_temp.arriv_time_i = (Airline_temp.arriv_time[9] - 48) * 1000 + (Airline_temp.arriv_time[10] - 48) * 100 + (Airline_temp.arriv_time[12] - 48) * 10 + (Airline_temp.arriv_time[13] - 48);

        AirlineVector.push_back(Airline_temp);
    }
    //读入正确性检查
    for (int i = 1; i <= 9; i++)
    {
        cout << AirlineVector[i].airline_id << ' ' << AirlineVector[i].depart_date_i << ' ' << AirlineVector[i].depart_time_i << ' ' << AirlineVector[i].arriv_time_i << endl;
    }
    cout << "data line:" << AirlineVector.size() << endl;
    return;
}
int main()
{
    ReadData();
    Readdata_in_list_fare();

    // system("pause");
    return 0;
}
