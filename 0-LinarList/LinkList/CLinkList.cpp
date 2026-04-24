#include <iostream>
using namespace std;

typedef struct CLNode
{
    int data;
    CLNode *next;
}CLNode, *CLinkList;
//循环单链表的初始化
bool InitCLinkList(CLinkList &L)
{
    L = new CLNode;
    if(L == nullptr)
    {
        return false;
    }
    L->next = L;
    return true;
}
//判断一个循环单链表是否为空
bool isEmpty(CLinkList L)
{
    if(L->next == L)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//判断循环单链表的结点p是否为表尾结点
bool isTail(CLinkList L, CLNode *p)
{
    if(p->next == L)
    {
        return true;
    }
    else
    {
        return false;
    }
}