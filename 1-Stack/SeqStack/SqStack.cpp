#include <iostream>
using namespace std;

#define MaxSize 10

struct SqStack
{
    int data[MaxSize];
    int top;
};

void InitSqStack(SqStack &S)
{
    S.top = -1;
}

bool isSqStackEmpty(SqStack S)
{
    if(S.top == -1)
    {
        return true;
    }
    else {
        return false;
    }
}
//进栈
bool Push(SqStack &S, int e)
{
    if(S.top == MaxSize-1)
    {
        cout<<"栈满，进栈失败"<<endl;
        return false;
    }
    S.top++;
    S.data[S.top] = e;
    return true;
}

//出栈
bool Pop(SqStack &S, int &e)
{
    if(S.top == -1)//栈空
    {
        cout<<"栈空，出栈失败"<<endl;
        return false;
    }
    e = S.data[S.top];
    S.top--;
    return true;
}

//读取栈顶元素
bool GetTop(SqStack &S, int &e)
{
    if(S.top == -1)//栈空
    {
        cout<<"栈空，读取栈顶元素失败"<<endl;
        return false;
    }
    e = S.data[S.top];
    return true;
}

int main()
{
    SqStack S;
    int e;

    InitSqStack(S);
    cout << "顺序栈初始化成功" << endl;

    Push(S, 10);
    Push(S, 20);
    Push(S, 30);
    cout << "入栈: 10, 20, 30" << endl;

    if(GetTop(S, e))
    {
        cout << "栈顶元素: " << e << endl;
    }

    cout << "出栈顺序: ";
    while(Pop(S, e))
    {
        cout << e << " ";
    }
    cout << endl;

    return 0;
}

