#include <iostream>
using namespace std;
#define MaxSize 100

struct TreeNode{
    int value;
    bool isEmpty;
};

bool InitSqBinaryTree(TreeNode a[])
{
    for(int i = 0; i < MaxSize ; i++)
    {
        a[i].isEmpty = true;
        a[i].value = 0;
    }
    return true;
}