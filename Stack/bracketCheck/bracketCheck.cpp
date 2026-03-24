#include <iostream>
#include <cmath>
using namespace std;

typedef struct LinkNode{
    char data;
    LinkNode *next;
}*LinkStack;

//初始化链栈
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
//判断栈是否为空
bool isStackEmpty(LinkStack S)
{
    return S->next == nullptr;
}

//入栈
bool Push(LinkStack &S, char a)
{
    LinkNode *newNode = new LinkNode;
    if(newNode == nullptr)
    {
        return false;
    }
    newNode->data = a;
    newNode->next = S->next;
    S->next = newNode;
    return true;
}

//出栈
bool Pop(LinkStack &S, char &a)
{
    if(S->next == nullptr)
    {
        cout<<"栈空，出栈失败"<<endl;
        return false;
    }
    LinkNode *p = S->next;
    a = p->data;
    S->next = p->next;
    delete p;
    return true;
}

//销毁链栈
void DestroyStack(LinkStack &S) {
    char temp;
    while (!isStackEmpty(S)) {
        Pop(S, temp); // 依次弹出并释放所有数据节点
    }
    delete S; // 释放头结点
    S = nullptr;
}

bool bracketCheck(char str[],int length)
{
    LinkStack S;
    InitLinkStack(S);
    bool isMatch = true;
    for (int i = 0; i<length; i++)
    {
        if(str[i] == '(' || str[i] == '[' || str[i] == '{' )
        {
            Push(S, str[i]);
        }
        else if(str[i] == ')' || str[i] == ']' || str[i] == '}' )
        {
            if(isStackEmpty(S))
            {
                isMatch = false;
                break;
            }
            else
            {
                char topElem;
                Pop(S, topElem); 
                if((str[i] == ')' && topElem != '(')||(str[i] == ']' && topElem != '[')||(str[i] == '}' && topElem != '{'))
                {
                    isMatch = false;
                    break;
                }
            }
        }
    }
    if(!isStackEmpty(S))
    {
        isMatch =false;
    }
    DestroyStack(S);
    return isMatch;
}