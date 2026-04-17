#include<iostream>
using namespace std;
#define MaxVertexNum 100//顶点数目最大值

struct MGraph{
    char Vex[MaxVertexNum]; //顶点表
    bool Edge[MaxVertexNum][MaxVertexNum]; //邻接矩阵，边表
    int vexnum, edgenum; //顶点数，边数（弧数）
};
