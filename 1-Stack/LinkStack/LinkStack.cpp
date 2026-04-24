#include <iostream>
using namespace std;

// 链栈节点定义
typedef struct LinkNode{
    int data;
    LinkNode *next;
} *LinkStack;

// 初始化链栈
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

// 入栈
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

// 出栈
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

// 获取栈顶元素
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

int main() {
    LinkStack S;
    int e;
    
    // 初始化栈
    if (InitLinkStack(S)) {
        cout << "栈初始化成功" << endl;
    } else {
        cout << "栈初始化失败" << endl;
        return 1;
    }
    
    // 入栈操作
    Push(S, 1);
    Push(S, 2);
    Push(S, 3);
    cout << "入栈元素: 1, 2, 3" << endl;
    
    // 获取栈顶元素
    if (GetTop(S, e)) {
        cout << "栈顶元素: " << e << endl;
    }
    
    // 出栈操作
    cout << "出栈顺序: ";
    while (Pop(S, e)) {
        cout << e << " ";
    }
    cout << endl;
    
    // 测试空栈
    if (!GetTop(S, e)) {
        cout << "栈为空，测试成功" << endl;
    }
    
    return 0;
}
