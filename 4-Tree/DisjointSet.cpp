#include<iostream>
using std::cout;
using std::endl;

#define SIZE 100
//并查集的初始化
void Initial(int S[])
{
    for(int i = 0; i < SIZE ; i++)
    {
        S[i] = -1;
    }
}

//在并查集中查找并返回包含元素x的树的根
int Find(int S[], int x)
{
    while(S[x] >= 0)
    {
        x = S[x];
    }
    return x;
}

//先查找到根结点，再压缩路径
int FindPro(int S[], int x)
{
    int root = x;
    while(S[root] >= 0)//循环找到根
    {
        root = S[root];
    }
    while(x != root)//压缩路径
    {
        int t = S[x];
        S[x] = root;
        x = t;
    }
    return root;
}

//合并两个不相交的子集合的操作。当要合并两个元素所在的集合时，
//需要先找到这两个元素的根，然后让一棵子树的根指向另一棵子集树的根
void Union1(int S[], int root1, int root2)
{
    if(root1 == root2)
    {
        return;
    }
    S[root2] = root1;
}

//小树合并到大树中
void Union2(int S[], int root1, int root2)
{
    if(root1 == root2)
    {
        return;
    }
    if(S[root2] > S[root1])//root结点为负，当root2的结点更少时
    {
        S[root1] = S[root1] + S[root2];//累加结点数量
        S[root2] = root1;//小树合并到大树中
    }
    else {
        S[root2] = S[root1] + S[root2];//累加结点数量
        S[root1] = root2;//小树合并到大树中
    }
}

int main()
{
    int S[SIZE];
    Initial(S);

    Union2(S, 0, 1);
    Union2(S, 2, 3);
    Union2(S, 0, 2);

    cout << "Find(3) = " << FindPro(S, 3) << endl;
    cout << "Find(1) = " << FindPro(S, 1) << endl;

    return 0;
}