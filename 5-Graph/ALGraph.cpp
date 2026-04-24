#include<iostream>
using namespace std;
#define MaxVertexNum 100 //图中顶点数目的最大值
//Adjacency List Graph（邻接表图）
struct ArcNode//边表结点
{
    int adjvex; //该弧指向的顶点的位置
    ArcNode *nextArc; //指向下一条弧的指针
};

typedef struct VNode//顶点表结点
{
    int data;//顶点信息
    ArcNode *firstArc;//指向第一条依附该顶点的弧的指针
}VNode, AdjList[MaxVertexNum];

struct ALGraph
{
    AdjList vertices; //邻接表
    int vexnum, arcnum;//顶点和弧的数量
};

int main()
{
    ALGraph G;
    G.vexnum = 4;
    G.arcnum = 4;

    for(int i = 0; i < G.vexnum; i++)
    {
        G.vertices[i].data = i + 1;
        G.vertices[i].firstArc = nullptr;
    }

    // 添加边: 1->2, 1->3, 2->4, 3->4
    ArcNode *a1 = new ArcNode{1, nullptr};
    ArcNode *a2 = new ArcNode{2, nullptr};
    G.vertices[0].firstArc = a1;
    a1->nextArc = a2;

    ArcNode *a3 = new ArcNode{3, nullptr};
    G.vertices[1].firstArc = a3;

    ArcNode *a4 = new ArcNode{3, nullptr};
    G.vertices[2].firstArc = a4;

    cout << "邻接表图:" << endl;
    for(int i = 0; i < G.vexnum; i++)
    {
        cout << "顶点 " << G.vertices[i].data << ": ";
        ArcNode *p = G.vertices[i].firstArc;
        while(p)
        {
            cout << "-> " << G.vertices[p->adjvex].data << " ";
            p = p->nextArc;
        }
        cout << endl;
    }

    // 释放内存
    for(int i = 0; i < G.vexnum; i++)
    {
        ArcNode *p = G.vertices[i].firstArc;
        while(p)
        {
            ArcNode *temp = p;
            p = p->nextArc;
            delete temp;
        }
    }

    return 0;
}