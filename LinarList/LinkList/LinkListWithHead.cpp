#include <cstddef>
#include <iostream>
using namespace std;

typedef struct LNode
{
    int data;
    LNode *next;
}LNode, *LinkList;

bool InitLinkListWithHead(LinkList &L)//初始化带头结点的单链表
{
    L = new LNode;
    if(L == nullptr)
    {
        return false;
    }
    L->next = nullptr;
    return true;
}

//按位查找，返回第i个元素（带头结点）
LNode *GetElem(LinkList L, int i)
{
    if(i<0)
    {
        return nullptr;
    }
    LNode *p = L;
    int j = 0;
    while(p != nullptr && j<i)
    {
        p = p->next;
        j++;
    }
    return p;
}

//按值查找，找到第一个数据域为e的结点，并返回这个结点
LNode *LocateElem(LinkList L, int e)
{
    LNode *p =L->next;
    while(p->data != e && p != nullptr)
    {
        p = p->next;
    }
    return p;
}

//求单链表的长度
int length(LinkList L)
{
    int len = 0;
    LNode *p = L;
    while(p->next != nullptr)
    {
        p = p->next;
        len++;
    }
    return len;
}

//在p结点后插入一个元素
bool InsertNextNode(LNode *p, int e)
{
    if(p == nullptr)
    {
        return false;
    }
    LNode *newNode = new LNode;
    if(newNode == nullptr)
    {
        cout<<"新结点内存分配失败"<<endl;
        return false;
    }
    newNode->data = e;
    newNode->next = p->next;
    p->next = newNode;
    return true;
}

bool ListInsert(LinkList &L,int i,int e)//在位序i处插入元素e
{
    if(i < 1)
    {
        return false;
    }
    LNode *p = GetElem(L,i-1);//找到第i-1个结点
    if (p == nullptr)
    {
        return false;
    }
    return InsertNextNode(p, e);
}

bool InsertPriorNode(LinkList &L, LNode *p, LNode *q)//在p结点前插入q结点
{
    if(p == nullptr || q == nullptr)
    {
        return false;
    }

    // 如果 p 是头结点，不能使用交换数据的方法
    if(p == L)
    {
        return false;  // 或者使用其他方法
    }
    
    q->next = p->next;
    p->next = q;
    int temp = p->data;
    p->data = q->data;
    q->data = temp;
    return true;
}

bool ListDelete(LinkList &L, int i, int &e)//删除位序为i的结点，并返回该结点的值
{
    if(i < 1)
    {
        return false;
    }
    LNode *p = GetElem(L, i-1);//找到第i-1个结点
    if(p == nullptr)
    {
        return false;
    }

    if(p->next == nullptr)
    {
        return false;
    }
    LNode *deleteNode = p->next;
    e = deleteNode->data;
    p->next = deleteNode->next;
    delete deleteNode;
    return true;
}

bool DeleteNode(LNode *p)//删除某个结点（偷梁换柱法）
{
    if(p == nullptr)
    {
        return false;
    }
    //如果p是最后一个结点，无法使用这种方法
    if(p->next == nullptr)
    {
        return false;
    }
    LNode *deleteNode = p->next;
    p->data = deleteNode->data;
    p->next = deleteNode->next;
    delete deleteNode;

    return true;
}

bool DestroyList(LinkList &L)//释放链表
{
    if(L == nullptr)
    {
        return false;
    }
    LNode *current = L;
    LNode *temp;
    while(current != nullptr)
    {
        temp = current;
        current = current->next;
        delete temp;
    }
    L = nullptr;
    return true;
}

bool PrintLinkList(LinkList L)
{
    if(L == nullptr)
    {
        return false;
    }

    LNode *p = L->next;
    while(p != nullptr)
    {
        cout<<p->data<<endl;
        p = p->next;
    }
    return true;
}

LinkList List_TailInsert_WithFunc(LinkList &L)
{
    int x;
    InitLinkListWithHead(L);
    LNode *tailNode = L;
    cout<<"调用封装好的函数的尾插法，请输入链表各结点的值(输入666结束):"<<endl;
    cin >> x;
    while(x != 666)
    {
        if(!InsertNextNode(tailNode, x))
        {
            cout<<"插入结点失败"<<endl;
            DestroyList(L);
            return nullptr;
        }
        tailNode = tailNode->next;
        cin >> x;
    }
    return L;
}

LinkList List_TailInsert_WithOutFunc(LinkList &L)
{
    int x;
    L = new LNode;
    if(L == nullptr)
    {
        return nullptr;
    }
    L->next = nullptr;
    LNode *tailNode = L;
    cout<<"不调用封装好的函数的尾插法，请输入链表各结点的值(输入666结束):"<<endl;
    cin >> x;
    while(x != 666)
    {
        LNode *newNode = new LNode;
        if(newNode == nullptr)
        {
            cout<<"插入失败"<<endl;
            DestroyList(L);
            return nullptr;
        }
        newNode->data = x;
        tailNode->next = newNode;
        tailNode = tailNode->next;
        cin >> x;
    }
    return L;
}

LinkList List_HeadInsert_WithFunc(LinkList &L)
{
    int x;
    InitLinkListWithHead(L);
    LNode *headNode = L;
    cout<<"调用封装好的函数的头插法，请输入链表各结点的值(输入666结束):"<<endl;
    cin >> x;
    while(x != 666)
    {
        if(!InsertNextNode(headNode, x))
        {
            cout<<"插入结点失败"<<endl;
            DestroyList(L);
            return nullptr;
        }
        cin >> x;
    }
    return L;
}


LinkList List_HeadInsert_WithOutFunc(LinkList &L)
{
    int x;
    //初始化
    L = new LNode;
    if (L == nullptr)
    {
        return nullptr;
    }
    L->next = nullptr;
    //
    LNode* headNode = L;
    cout<<"不调用封装好的函数的头插法，请输入链表各结点的值(输入666结束):"<<endl;
    cin >> x;
    while(x != 666)
    {
        LNode *newNode =new LNode;
        if(newNode == nullptr)
        {
            cout<<"插入失败"<<endl;
            DestroyList(L);
            return nullptr;
        }
        newNode->data = x;
        newNode->next = headNode->next;
        headNode->next = newNode;
        cin >> x;
    }
    return L;
}

bool ReverseLink(LinkList &L)
 {
    LNode *p = L->next;
    L->next = nullptr;
    LNode *q;
    while( p!= nullptr)
    {
        q = p->next;
        p->next = L->next;
        L->next = p;
        p = q;
    }
    return true;
 }

int main()
{
    LinkList L1, L2, L3, L4;
    List_TailInsert_WithFunc(L1);
    PrintLinkList(L1);
    cout<<"链表的长度为"<<length(L1)<<endl;
    ReverseLink(L1);
    PrintLinkList(L1);
    DestroyList(L1);

    List_TailInsert_WithOutFunc(L2);
    PrintLinkList(L2);
    cout<<"链表的长度为"<<length(L2)<<endl;
    DestroyList(L2);

    List_HeadInsert_WithFunc(L3);
    PrintLinkList(L3);
    cout<<"链表的长度为"<<length(L3)<<endl;
    DestroyList(L3);

    List_HeadInsert_WithOutFunc(L4);
    PrintLinkList(L4);
    cout<<"链表的长度为"<<length(L4)<<endl;
    DestroyList(L4);
    return 0;
}