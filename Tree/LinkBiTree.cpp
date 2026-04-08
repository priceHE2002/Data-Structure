#include<iostream>
using std::cin;
using std::cout;
using std::endl;

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

//求树的深度
int treeDepth(BiTree T)
{
    if(T == nullptr)
    {
        return 0;
    }
    //树的深度 = max（左子树，右子树）+ 1
    else {
        int l = treeDepth(T->leftChild);
        int r = treeDepth(T->rightChild);
        return r > l ? r + 1 : l + 1;
    }
}

//辅助队列结点
struct LinkNode
{
    BiTNode *data;
    LinkNode *next;
};

//辅助队列
struct LinkQueue
{
    LinkNode *front, *rear;
};

//辅助队列的初始化
bool InitLinkQueue(LinkQueue &Q)
{
    LinkNode *node = new LinkNode;
    if(node == nullptr)
    {
        return false;
    }
    node->next = nullptr;
    Q.front = Q.rear = node;
    return true;
}

//辅助队列判空
bool IsLinkQueueEmpty(LinkQueue Q)
{
    return Q.front == Q.rear;
}

//入队
bool enLinkQueue(LinkQueue &Q, BiTNode *e)
{
    LinkNode *newNode = new LinkNode;
    if(newNode == nullptr)
    {
        cout<<"新结点内存分配失败，入队失败"<<endl;
        return false;
    }
    newNode->data =e;
    newNode->next = nullptr;
    Q.rear->next = newNode;
    Q.rear = Q.rear->next;
    return true;
}

//出队，出队
bool deLinkQueue(LinkQueue &Q, BiTNode *&e)
{
    if(Q.front == Q.rear)
    {
        cout<<"空列为空，出队失败"<<endl;
        return false;
    }
    LinkNode *p = Q.front->next;
    e = p->data;
    Q.front->next = p->next;
    if(Q.rear == p)
    {
        Q.rear = Q.front;
    }
    delete p;
    return true;
}

bool DestroyLinkQueue(LinkQueue &Q)
{
    LinkNode *p = Q.front;  //从队头开始
    while(p != nullptr)     //遍历所有结点
    {
        Q.front = p->next;  //先保存下一个结点
        delete p;           //删除当前结点
        p = Q.front;        //移动到下一个结点
    }
    Q.front = Q.rear = nullptr;  //清空队列指针
    return true;
}

//层序遍历
void levelOrder(BiTree T)
{
    LinkQueue Q;
    InitLinkQueue(Q);
    BiTree p;
    enLinkQueue(Q, T);
    while(!IsLinkQueueEmpty(Q))
    {
        deLinkQueue(Q, p);
        visit(p);
        if(p->leftChild != nullptr)
        {
            enLinkQueue(Q, p->leftChild);
        }
        if(p->rightChild != nullptr)
        {
            enLinkQueue(Q, p->rightChild);
        }
    }
    DestroyLinkQueue(Q);
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

    cout << "层序遍历: ";
    levelOrder(root);
    cout << endl;

    return 0;
}