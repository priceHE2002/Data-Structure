#include<iostream>
#include<queue>
using namespace std;
#define MaxVertexNum 100//顶点数目最大值

//无向无权图
struct MGraph{
    char Vex[MaxVertexNum]; //顶点表
    bool Edge[MaxVertexNum][MaxVertexNum]; //邻接矩阵，边表
    int vexnum, edgenum; //顶点数，边数（弧数）
};

//初始化
void InitMGraph(MGraph &G) 
{
    G.vexnum = 0;
    G.edgenum = 0;

    // 遍历矩阵，将所有可能的边初始化为 0（代表无边）
    for (int i = 0; i < MaxVertexNum; i++) {
        for (int j = 0; j < MaxVertexNum; j++) {
            G.Edge[i][j] = false; 
        }
    }
}

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
void InsertEdge(MGraph &G, int x, int y)
{
    if(x < 0 || x >= G.vexnum || y < 0 || y >= G.vexnum)
    {
        cout<<"x或y输入非法"<<endl;
        return;
    }

    // 优化：检查边是否已经存在，避免重复增加边数
    if (!G.Edge[x][y]) 
    {
        G.Edge[x][y] = true;
        G.Edge[y][x] = true;
        G.edgenum++;
    }
}

//在图G中插入顶点X
void InsertVertex(MGraph &G, char X) 
{
    //检查空间是否已满
    if (G.vexnum >= MaxVertexNum) {
        return; 
    }
    //插入顶点信息
    G.Vex[G.vexnum] = X;
    //顶点数加 1
    G.vexnum++;
}

// 求图中顶点x的第一个邻接点，若有则返回顶点号，若x没有邻接点或越界，则返回-1
int FirstNeighbor(MGraph &G, int x)
{
    // 检查索引合法性
    if (x < 0 || x >= G.vexnum) 
    {
        return -1; 
    }

    // 直接在邻接矩阵第 x 行中查找第一个邻接点
    for (int j = 0; j < G.vexnum; j++)
    {
        if (G.Edge[x][j] == true) 
        {
            return j;
        }
    }
    // 如果遍历完都没有找到，说明是孤立点
    return -1;
}

// 假设图G中顶点y是顶点x的一个邻接点，返回除y之外顶点x的下一个邻接点的顶点号
int NextNeighbor(MGraph &G, int x, int y) 
{
    // 检查索引合法性，并确保y确实是x的邻接点
    if (x < 0 || x >= G.vexnum || y < 0 || y >= G.vexnum || G.Edge[x][y] == false) 
    {
        return -1; 
    }

    // 直接从 y 的下一个位置开始寻找下一个邻接点
    for (int j = y + 1; j < G.vexnum; j++)
    {
        if (G.Edge[x][j] == true) 
        {
            return j;
        }
    }

    // 如果遍历到最后都没找到，说明 y 是 x 的最后一个邻接点
    return -1;
}

bool visited[MaxVertexNum]; //访问标记数组

void BFS(MGraph &G, int v)
{

    queue<int> q; //辅助队列q
    visited[v] = true;//将v的访问标记数组设置为true
    cout<< v << " ";//访问v结点，并打印该结点
    q.push(v);//v结点入队

    while(!q.empty())//当队列q不为空时循环执行以下操作
    {
        int u = q.front();//声明整型变量u，用于接收队头元素
        q.pop();
        for(int w = FirstNeighbor(G,u); w>= 0 ; w = NextNeighbor(G, u, w))
        {
            if(!visited[w])
            {
                cout<< w << " ";//访问w结点；并打印该结点
                visited[w] = true;//将w结点的访问标记数组设置为true
                q.push(w);//w结点入队
            }
        }
    }
}

//对非连通图G进行广度优先遍历
void BFSTraverse(MGraph &G)
{
    //将访问标记数组重置为全false
    for(int i = 0; i < MaxVertexNum; i++) {
        visited[i] = false;
    }
    for(int j = 0; j < G.vexnum; j++)
    {
        if(!visited[j])
        {
            BFS(G, j);
        }
    }

}

int main()
{
    MGraph G;
    
    // 1. 初始化图
    InitMGraph(G);
    
    // 2. 插入顶点 (索引分别是 0:A, 1:B, 2:C, 3:D, 4:E)
    InsertVertex(G, 'A'); // 0
    InsertVertex(G, 'B'); // 1
    InsertVertex(G, 'C'); // 2
    InsertVertex(G, 'D'); // 3
    InsertVertex(G, 'E'); // 4
    
    /* 构建的图结构大致如下：
       A(0) --- B(1)
        |        |
       C(2) --- D(3)
        |
       E(4)
    */

    // 3. 插入边
    InsertEdge(G, 0, 1); // A - B
    InsertEdge(G, 0, 2); // A - C
    InsertEdge(G, 1, 3); // B - D
    InsertEdge(G, 2, 3); // C - D
    InsertEdge(G, 2, 4); // C - E
    
    cout << "✅ 图初始化完成！" << endl;
    cout << "📌 顶点数: " << G.vexnum << "，边数: " << G.edgenum << "\n" << endl;
    
    // ================= 开始测试 BFS =================

    // --- 测试 1: 以 A(0) 为起点进行广度优先遍历 ---
    //将访问标记数组重置为全false
    for(int i = 0; i < MaxVertexNum; i++) {
        visited[i] = false;
    }
    cout << "--- 测试 BFS (起点为 A, 索引 0) ---" << endl;
    cout << "实际 BFS 遍历序列 (索引): ";
    BFS(G, 0); 
    cout << "\n预期 BFS 遍历序列 (索引): 0 1 2 3 4" << endl;
    cout << "过程解析: 从 0 开始 -> 找到第一层邻居 1, 2 -> 找 1 的邻居 3 -> 找 2 的邻居 4" << endl;

    // --- 测试 2: 以 C(2) 为起点进行广度优先遍历 ---
    //将访问标记数组重置为全false
    for(int i = 0; i < MaxVertexNum; i++) {
        visited[i] = false;
    }
    cout << "\n--- 测试 BFS (起点为 C, 索引 2) ---" << endl;
    cout << "实际 BFS 遍历序列 (索引): ";
    BFS(G, 2); 
    cout << "\n预期 BFS 遍历序列 (索引): 2 0 3 4 1" << endl;
    cout << "过程解析: 从 2 开始 -> 找到第一层所有邻居 0, 3, 4 -> 再找 0 的邻居 1" << endl;

    return 0;
}