#include <iostream>
using namespace std;

typedef struct DNode
{
    int data;
    DNode *prior, *next;
}DNode,*DLinkList;

//双链表的初始化
bool InitDLinkList(DLinkList &L)
{
    L = new DNode;
    if(L == nullptr)
    {
        return false;
    }
    L->next = nullptr;
    L->prior = nullptr;
    return true;
}
//在p结点后插入s结点
bool InsertNextDNode(DNode *p, DNode *s)
{
    if(p == nullptr || s == nullptr)
    {
        return false;
    }
    if(p->next == nullptr)
    {
        p->next = s;
        s->prior = p;
        s->next = nullptr;
        return true;
    }
    else 
    {
        s->next = p->next;
        p->next->prior = s;
        s->prior = p;
        p->next = s;
        return true;
    }
}

//删除p结点的后继结点
bool DeleteNextDNode(DNode *p)
{
    if(p == nullptr)
    {
        return false;
    }
    if(p->next == nullptr)
    {
        return false;
    }
    else 
    {
        DNode *q = p->next;
        p->next = q->next;
        if(q->next != nullptr)
        {
            q->next->prior = p;
        }
        delete q;
        return true;
    }
}

void DestroyDLinkList(DLinkList &L)
{
    while(L->next != nullptr)
    {
        DeleteNextDNode(L);
    }
    delete L;
    L = nullptr;
}

int main()
{
    DLinkList L;
    InitDLinkList(L);
    DestroyDLinkList(L);
}
