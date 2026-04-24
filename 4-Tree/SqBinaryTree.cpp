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

bool InsertTreeNode(TreeNode a[], int i, int value)
{
    if(i < 0 || i >= MaxSize)
    {
        cout << "位置越界" << endl;
        return false;
    }
    a[i].isEmpty = false;
    a[i].value = value;
    return true;
}

bool HasLeftChild(TreeNode a[], int i)
{
    int leftChild = 2 * i + 1;
    if(leftChild >= MaxSize || a[leftChild].isEmpty)
    {
        return false;
    }
    return true;
}

bool HasRightChild(TreeNode a[], int i)
{
    int rightChild = 2 * i + 2;
    if(rightChild >= MaxSize || a[rightChild].isEmpty)
    {
        return false;
    }
    return true;
}

bool GetLeftChild(TreeNode a[], int i, int &value)
{
    int leftChild = 2 * i + 1;
    if(leftChild >= MaxSize || a[leftChild].isEmpty)
    {
        return false;
    }
    value = a[leftChild].value;
    return true;
}

bool GetRightChild(TreeNode a[], int i, int &value)
{
    int rightChild = 2 * i + 2;
    if(rightChild >= MaxSize || a[rightChild].isEmpty)
    {
        return false;
    }
    value = a[rightChild].value;
    return true;
}

bool GetParent(TreeNode a[], int i, int &value)
{
    if(i <= 0 || i >= MaxSize || a[i].isEmpty)
    {
        return false;
    }
    int parent = (i - 1) / 2;
    if(parent < 0 || a[parent].isEmpty)
    {
        return false;
    }
    value = a[parent].value;
    return true;
}

bool IsLeaf(TreeNode a[], int i)
{
    if(i < 0 || i >= MaxSize || a[i].isEmpty)
    {
        return false;
    }
    return !HasLeftChild(a, i) && !HasRightChild(a, i);
}

void PrintSqBinaryTree(TreeNode a[])
{
    cout << "顺序二叉树（数组表示：" << endl;
    for(int i = 0; i < MaxSize; i++)
    {
        if(!a[i].isEmpty)
        {
            cout << "位置 " << i << ": " << a[i].value;
            if(HasLeftChild(a, i))
            {
                int leftValue;
                GetLeftChild(a, i, leftValue);
                cout << ", 左孩子: " << leftValue;
            }
            if(HasRightChild(a, i))
            {
                int rightValue;
                GetRightChild(a, i, rightValue);
                cout << ", 右孩子: " << rightValue;
            }
            if(IsLeaf(a, i))
            {
                cout << ", 叶子结点";
            }
            cout << endl;
        }
    }
}

int main()
{
    TreeNode tree[MaxSize];
    InitSqBinaryTree(tree);

    InsertTreeNode(tree, 0, 1);
    InsertTreeNode(tree, 1, 2);
    InsertTreeNode(tree, 2, 3);
    InsertTreeNode(tree, 3, 4);
    InsertTreeNode(tree, 4, 5);

    PrintSqBinaryTree(tree);

    int parentVal;
    if(GetParent(tree, 4, parentVal))
    {
        cout << "结点5的双亲: " << parentVal << endl;
    }

    return 0;
}