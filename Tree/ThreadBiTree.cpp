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
    //该结点的左子树为空，建立前驱线索
    if(q->leftChild == nullptr)
    {
        q->leftChild = pre;
        q->ltag = 1;
    }
    //该结点的前驱线索结点的右子树为空，建立前驱线索结点的后继线索
    if(pre != nullptr && pre->rightChild == nullptr)
    {
        pre->rightChild = q;
        pre->rtag = 1;
    }
    pre = q;
}

//中序遍历二叉树，一边遍历一边线索化
void InThread(ThreadBiTree &T)
{
    if(T != nullptr)
    {
        InThread(T->leftChild);
        visit(T);
        InThread(T->rightChild);
    }
}

//中序线索化二叉树T
void CreateInThread(ThreadBiTree &T)
{
    pre = nullptr;  // 重置 pre
    InThread(T);
    
    // 处理最后一个结点的右线索
    if(pre != nullptr && pre->rightChild == nullptr)
    {
        pre->rtag = 1;
        // pre->rightChild 保持为 nullptr，表示没有后继
    }
}

// 找到以 P 为根的子树中，第一个被中序遍历的结点
ThreadNode *FirstNode(ThreadNode *P)
{
    if(P == nullptr) return nullptr;  // 空指针检查
    
    //循环找到最左下角的结点
    while(P->ltag == 0)
    {
        P=P->leftChild;
    }
    return P;
}

//在中序线索二叉树中找到中序遍历的后继结点
ThreadNode *NextNode(ThreadNode *P)
{
    if(P == nullptr) return nullptr;  // 空指针检查
    
    //右子树中最左下角的结点
    if(P->rtag == 0)
    {
        return FirstNode(P->rightChild);
    }
    //如果 P->rtag == 1，直接返回后继线索
    else {
        return P->rightChild;
    }
}

//对中序线索二叉树进行中序遍历
void InOrderThread(ThreadBiTree T)
{
    if(T == nullptr) return;  // 空树检查
    
    for(ThreadNode *p = FirstNode(T); p != nullptr; p = NextNode(p) )
    {
        cout << p->data << " ";  // 直接打印，不使用 visit（visit 会修改线索）
    }
}

// 找到以 P 为根的子树中，最后一个被中序遍历的结点
ThreadNode *LastNode(ThreadNode *P)
{
    if(P == nullptr) return nullptr;  // 空指针检查
    //循环找到最右下角的结点
    while(P->rtag == 0)
    {
        P=P->rightChild;
    }
    return P;
}

//先序遍历二叉树，一边遍历一边线索化
void PreThread(ThreadBiTree &T)
{
    if(T != nullptr)
    {
        visit(T);
        // 只有当左子树不是线索时才递归
        if(T->ltag == 0)
        {
            PreThread(T->leftChild);
        } 

        if(T->rtag == 0)
        {
            PreThread(T->rightChild);
        }

    }
}

//先序线索化二叉树T
void CreatePreThread(ThreadBiTree &T)
{
    pre = nullptr;  // 重置 pre
    PreThread(T);
    // 处理最后一个结点的右线索
    if(pre != nullptr && pre->rightChild == nullptr)
    {
        pre->rtag = 1;
        // pre->rightChild 保持为 nullptr，表示没有后继
    }
}

//后序遍历二叉树，一边遍历一边线索化
void PostThread(ThreadBiTree &T)
{
    if(T != nullptr)
    {
        PostThread(T->leftChild);
        PostThread(T->rightChild);
        visit(T);
    }
}

//后序线索化二叉树T
void CreatePostThread(ThreadBiTree &T)
{
    pre = nullptr;  // 重置 pre
    PostThread(T);
    // 处理最后一个结点的右线索
    if(pre != nullptr && pre->rightChild == nullptr)
    {
        pre->rtag = 1;
        // pre->rightChild 保持为 nullptr，表示没有后继
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
    CreateInThread(root);
    
    // 中序遍历线索二叉树
    cout << "中序线索化后遍历: ";
    InOrderThread(root);
    cout << endl;
    
    return 0;
}