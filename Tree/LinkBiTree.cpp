#include <cstddef>
#include<iostream>
using namespace std;

typedef struct BiTNode{
    int value;
    BiTNode *leftChild, *rightChild;
}BiTNode, *BiTree;

//初始化二叉树
bool InitBiTree(BiTree &root)
{
    root = new BiTNode;
    if (root == nullptr)
    {
        cout<<"内存分配失败"<<endl;
        return false;
    }
    root->leftChild = nullptr;
    root->rightChild = nullptr;
    root->value = 0;
    return true;
}

//在某个父节点后，插入数据为 data 的节点，child = l 时为左孩子，child = r 时为右孩子
bool InsertNewNode(BiTNode *parent, int data, char child)
{   
    // 检查 parent 是否为空
    if(parent == nullptr)
    {
        cout<<"parent 结点为空"<<endl;
        return false;
    }

    // 检查 child 参数
    if(child != 'l' && child != 'r')
    {
        cout<<"child 输入非法（仅能为'r' or 'l'）"<<endl;
        return false;
    }

    // 插入左孩子
    if(child == 'l')
    {
        if(parent->leftChild == nullptr)
        {
            BiTNode *newNode = new BiTNode;
            if (newNode == nullptr)
            {
                cout<<"内存分配失败"<<endl;
                return false;
            }
            newNode->leftChild = nullptr;
            newNode->rightChild = nullptr;
            newNode->value = data;
            parent->leftChild = newNode;
            return true;
        }
        else
        {
            cout<<"parent 左孩子结点不为空，插入失败"<<endl;
            return false;
        }
    }
    // 插入右孩子
    else if(child == 'r')
    {
        if(parent->rightChild == nullptr)
        {
            BiTNode *newNode = new BiTNode;
            if (newNode == nullptr)
            {
                cout<<"内存分配失败"<<endl;
                return false;
            }
            newNode->leftChild = nullptr;
            newNode->rightChild = nullptr;
            newNode->value = data;
            parent->rightChild = newNode;
            return true;
        }
        else
        {
            cout<<"parent 右孩子结点不为空，插入失败"<<endl;
            return false;
        }
    }

    return false;
}

int main()
{
    BiTree root;
    InitBiTree(root);
    
    // 插入左孩子
    if(InsertNewNode(root, 10, 'l'))
    {
        cout << "左孩子插入成功" << endl;
    }
    
    // 插入右孩子
    if(InsertNewNode(root, 20, 'r'))
    {
        cout << "右孩子插入成功" << endl;
    }
    
    // 验证
    cout << "根结点值: " << root->value << endl;
    cout << "左孩子值: " << root->leftChild->value << endl;
    cout << "右孩子值: " << root->rightChild->value << endl;
    
    return 0;
}