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

// 全局变量 pre，记录前驱结点
ThreadNode *pre = nullptr;

//初始化线索二叉树，根结点的数据为 data
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

//访问某个结点，建立线索
void visit(ThreadNode *q)
{
    //左子树为空，建立前驱线索
    if(q->leftChild == nullptr)
    {
        q->leftChild = pre;
        q->ltag = 1;
    }
    //前驱结点的右子树为空，建立前驱结点的后继线索
    if(pre != nullptr && pre->rightChild == nullptr)
    {
        pre->rightChild = q;
        pre->rtag = 1;
    }
    pre = q;
}

//构造中序线索二叉树
void InThread(ThreadBiTree &T)
{
    if(T != nullptr)
    {
        InThread(T->leftChild);
        visit(T);
        InThread(T->rightChild);
    }
}

// 中序遍历线索二叉树（从头结点开始）
void InOrderThread(ThreadBiTree T)
{
    ThreadNode *p = T;
    
    // 找到最左下的结点（第一个结点）
    while(p->leftChild != nullptr && p->ltag == 0)
    {
        p = p->leftChild;
    }
    
    // 遍历所有结点
    while(p != nullptr)
    {
        cout << p->data << " ";
        
        // 如果右子树是线索，直接访问后继
        if(p->rtag == 1)
        {
            p = p->rightChild;
        }
        else
        {
            // 否则从右子树的最左下结点开始
            p = p->rightChild;
            while(p != nullptr && p->ltag == 0)
            {
                p = p->leftChild;
            }
        }
    }
}

int main()
{
    ThreadBiTree root;
    InitThreadBiTree(root, 1);
    
    // 构建二叉树
    InsertNewNode(root, 2, 'l');
    InsertNewNode(root, 3, 'r');
    InsertNewNode(root->leftChild, 4, 'l');
    InsertNewNode(root->leftChild, 5, 'r');
    InsertNewNode(root->rightChild, 6, 'r');
    
    // 线索化
    pre = nullptr;  // 重置 pre
    InThread(root);
    
    // 中序遍历线索二叉树
    cout << "中序线索化后遍历: ";
    InOrderThread(root);
    cout << endl;
    
    return 0;
}