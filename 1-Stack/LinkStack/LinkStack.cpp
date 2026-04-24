#include <iostream>
#include "LinkStack.h"
using namespace std;

bool InitLinkStack(LinkStack &S)
{
    S = new LinkNode;
    if(S == nullptr)
    {
        return false;
    }
    S->next = nullptr;
    return true;
}

bool Push(LinkStack &S, int e)
{
    LinkNode *newNode = new LinkNode;
    if(newNode == nullptr)
    {
        return false;
    }
    newNode->data = e;
    newNode->next = S->next;
    S->next = newNode;
    return true;
}

bool Pop(LinkStack &S, int &e)
{
    if(S->next == nullptr)
    {
        cout<<"栈空，无法出栈"<<endl;
        return false;
    }
    LinkNode *p = S->next;
    e = p->data;
    S->next = p->next;
    delete p;
    return true;
}

bool GetTop(LinkStack &S,int &e)
{
    if(S->next == nullptr)
    {
        cout<<"栈空"<<endl;
        return false;
    }
    e = S->next->data;
    return true;
}