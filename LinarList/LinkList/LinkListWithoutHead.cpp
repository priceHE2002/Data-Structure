#include <cstddef>
#include <iostream>

using namespace std;

typedef struct LNode
{
    int data;
    LNode *next;
}LNode, *LinkList;

bool InitLinkListWithoutHead(LinkList &L)
{
    L = nullptr;
    return true;
}

bool ListInsert(LinkList &L, int i, int e)//在位序为i的结点插入元素e
{
    if(i < 1)
    {
        return false;
    }
    if(i == 1)
    {
        LNode *newNode = new LNode;
        newNode->data = e;
        newNode->next = L;
        L = newNode;
        return true;
    }
    LNode *p;
    p = L;
    int j = 1;
    while(p != nullptr && j < i-1)
    {
        p = p->next;
        j++;
    }
    LNode *newNode = new LNode;
    newNode->data = e;
    newNode->next = p->next;
    p->next = newNode;
    return true;
}

LinkList List_HeadInsert_WithOutFunc(LinkList &L)
{
    L = nullptr;
    int x;
    cout<<"头插法建立不带头结点的链表，请输入链表元素（输入666时终止）："<<endl;
    cin>>x;
    while(x!=666)
    {
        LNode *newNode = new LNode;
        if(newNode == nullptr)
        {
            return nullptr;
        }
            newNode->data = x;
            newNode->next = L;
            L = newNode;
        cin>>x;
    }
    return L;
}

int main()
{
    LinkList L;
    InitLinkListWithoutHead(L);
    ListInsert(L,1, 1);
    return 0;
}