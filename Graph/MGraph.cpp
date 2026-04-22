#include<iostream>
using namespace std;
#define MaxVertexNum 100//顶点数目最大值

//无向图
struct MGraph{
    char Vex[MaxVertexNum]; //顶点表
    bool Edge[MaxVertexNum][MaxVertexNum]; //邻接矩阵，边表
    int vexnum, edgenum; //顶点数，边数（弧数）
};

//判断图G是否存在边(x , y)
bool Adjacent(MGraph &G, int x, int y) 
{
    if(x < 0 || x >= G.vexnum || y < 0 || y >= G.vexnum)
    {
        cout<<"x或y输入非法"<<endl;
        return false;
    }

    return G.Edge[x][y]; // 因为 Edge 本身就是 bool 类型
}

//列出图G中与结点x相邻接的所有的边
void Neighbors(MGraph &G, int x)
{
    for (int i = 0; i < G.vexnum; i++)
    {
        if(G.Edge[x][i])
        {
            cout<<G.Vex[i]<<" ";
        }
    }
    cout<<endl;
}

//在图G中插入边(x , y)
void InsertEdge(MGraph &G, int x,int y)
{
    if(x < 0 || x >= G.vexnum || y < 0 || y >= G.vexnum)
    {
        cout<<"x或y输入非法"<<endl;
        return;
    }

    //无向图，邻接矩阵为对称矩阵
    G.Edge[x][y] = true;
    G.Edge[y][x] = true;
    G.edgenum++;
}

int main()
{
    MGraph G;
    
    // 初始化图
    G.vexnum = 4;
    G.edgenum = 0;
    G.Vex[0] = 'A';
    G.Vex[1] = 'B';
    G.Vex[2] = 'C';
    G.Vex[3] = 'D';
    
    // 初始化邻接矩阵为 false
    for(int i = 0; i < G.vexnum; i++)
    {
        for(int j = 0; j < G.vexnum; j++)
        {
            G.Edge[i][j] = false;
        }
    }
    
    // 插入边
    InsertEdge(G, 0, 1); // A-B
    InsertEdge(G, 0, 2); // A-C
    InsertEdge(G, 1, 3); // B-D
    InsertEdge(G, 2, 3); // C-D
    
    cout << "图初始化完成，顶点数: " << G.vexnum << ", 边数: " << G.edgenum << endl;
    
    // 测试 Adjacent
    cout << "\n--- 测试 Adjacent ---" << endl;
    cout << "A 和 B 是否相邻? " << (Adjacent(G, 0, 1) ? "是" : "否") << endl;
    cout << "A 和 D 是否相邻? " << (Adjacent(G, 0, 3) ? "是" : "否") << endl;
    
    // 测试 Neighbors
    cout << "\n--- 测试 Neighbors ---" << endl;
    Neighbors(G, 0); // A 的邻居
    Neighbors(G, 1); // B 的邻居
    Neighbors(G, 2); // C 的邻居
    Neighbors(G, 3); // D 的邻居
    
    return 0;
}