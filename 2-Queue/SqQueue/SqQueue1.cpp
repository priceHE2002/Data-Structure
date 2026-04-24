//循环队列，牺牲一个存储单元的空间
#include <iostream>
using namespace std;

#define MaxSize 10
struct SqQueue
{
    int data[MaxSize];
    int front, rear;
};

bool InitSqQueue(SqQueue &Q)
{
    Q.front = Q.rear = 0;
    return true;
}

bool isQuereEmpty(SqQueue Q)
{
    if(Q.front == Q.rear)
    {
        return true;
    }
    else {
        return false;
    }
}
//队列入队
bool enQueue(SqQueue &Q, int e)
{
    if((Q.rear+1)%MaxSize == Q.front)
    {
        cout<<"队列已满，入队失败"<<endl;
        return false;
    }
    Q.data[Q.rear] = e;
    Q.rear = (Q.rear + 1) % MaxSize;
    return true;
}

//队列入队
bool deQueue(SqQueue &Q, int &e)
{
    if(Q.front == Q.rear)
    {
        cout<<"队列为空，出队失败"<<endl;
        return false;
    }
    e = Q.data[Q.front];
    Q.front = (Q.front + 1) % MaxSize;
    return true;
}

bool GetElem(SqQueue &Q, int &e)
{
    if(Q.front == Q.rear)
    {
        cout<<"队列为空，出队失败"<<endl;
        return false;
    }
    e = Q.data[Q.front];
    return true;
}

//获取当前队列的元素的个数（即当前队列的长度）
int Length(SqQueue Q)
{
    return (Q.rear + MaxSize -Q.front) % MaxSize;
}

int main()
{
    SqQueue Q;
    InitSqQueue(Q);
}