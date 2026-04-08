#include<iostream>
using namespace std;

typedef struct BiTNode{
    int value;
    BiTNode *leftChild, *rightChild;
}BiTNode, *BiTree;

//初始化二叉树，将根结点的值设置为data
bool InitBiTree(BiTree &root, int data)
{
    root = new BiTNode;
    if (root == nullptr)
    {
        cout<<"内存分配失败"<<endl;
        return false;
    }
    root->leftChild = nullptr;
    root->rightChild = nullptr;
    root->value = data;
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
//访问某个结点，该结点不为空则打印该结点的value
void visit(BiTNode *p)
{
    if(p == nullptr)
    {
        return;
    }
    cout<<p->value<<" ";
}

//先序遍历
void PreOrder(BiTree T)
{
    if(T != nullptr)
    {
        visit(T);
        PreOrder(T->leftChild);
        PreOrder(T->rightChild);
    }
}

//中序遍历
void InOrder(BiTree T)
{
    if(T != nullptr)
    {
        InOrder(T->leftChild);
        visit(T);
        InOrder(T->rightChild);
    }
}

//后序遍历
void PostOrder(BiTree T)
{
    if(T != nullptr)
    {
        PostOrder(T->leftChild);
        PostOrder(T->rightChild);
        visit(T);
    }
}

int main()
{
    BiTree root;
    InitBiTree(root, 1);
    
    // 构建二叉树
    InsertNewNode(root, 2, 'l');  // 左孩子
    InsertNewNode(root, 3, 'r');  // 右孩子
    InsertNewNode(root->leftChild, 4, 'l');  // 2 的左孩子
    InsertNewNode(root->leftChild, 5, 'r');  // 2 的右孩子
    InsertNewNode(root->rightChild, 6, 'r'); // 3 的右孩子
    
    // 先序遍历
    cout << "先序遍历: ";
    PreOrder(root);
    cout << endl;
    
    // 中序遍历
    cout << "中序遍历: ";
    InOrder(root);
    cout << endl;

    // 后序遍历
    cout << "后序遍历: ";
    PostOrder(root);
    cout << endl;

    return 0;
}