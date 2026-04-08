#include <iostream>
using std::cin;
using std::cout;
using std::endl;

//tag = 0 表示指向子结点，tag = 1 表示指向前驱（或后继）
typedef struct ThreadNode
{
    int data;
    ThreadNode *leftChild, *rightChild;
    int ltag, rtag;
}ThreadNode, *ThreadBiTree;

//初始化线索二叉树，根结点的数据为data
bool InitThreadBiTree(ThreadBiTree &root, int data)
{
    root = new ThreadNode;
    if (root == nullptr)
    {
        cout<<"内存分配失败"<<endl;
        return false;
    }
    root->leftChild = nullptr;
    root->rightChild = nullptr;
    root->data = data;
    root->ltag = 0;
    root->rtag = 0;
    return true;
}

//在某个父节点后，插入数据为 data 的节点，child = l 时为左孩子，child = r 时为右孩子
bool InsertNewNode(ThreadNode *parent, int data, char child)
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
            ThreadNode *newNode = new ThreadNode;
            if (newNode == nullptr)
            {
                cout<<"内存分配失败"<<endl;
                return false;
            }
            newNode->leftChild = nullptr;
            newNode->rightChild = nullptr;
            newNode->ltag = 0;
            newNode->rtag = 0;
            newNode->data = data;
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
            ThreadNode *newNode = new ThreadNode;
            if (newNode == nullptr)
            {
                cout<<"内存分配失败"<<endl;
                return false;
            }
            newNode->leftChild = nullptr;
            newNode->rightChild = nullptr;
            newNode->ltag = 0;
            newNode->rtag = 0;
            newNode->data = data;
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
//访问某个结点，该结点不为空则打印该结点的value
void visit(ThreadNode *p)
{
    if(p == nullptr)
    {
        return;
    }
    cout<<p->data<<" ";
}

//构造中序线索二叉树
void InThread(ThreadBiTree &T)
{
    
}