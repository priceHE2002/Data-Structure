#ifndef LINKSTACK_H
#define LINKSTACK_H

// 链栈节点定义
typedef struct LinkNode{
    int data;
    LinkNode *next;
} *LinkStack;

// 链栈操作函数声明
bool InitLinkStack(LinkStack &S);
bool Push(LinkStack &S, int e);
bool Pop(LinkStack &S, int &e);
bool GetTop(LinkStack &S,int &e);

#endif