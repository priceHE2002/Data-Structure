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