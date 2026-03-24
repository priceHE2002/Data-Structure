//循环队列，引入变量size记录队长
#include <iostream>
using namespace std;

#define MaxSize 10
struct SqQueue
{
    int data[MaxSize];
    int front, rear, size;
};

bool InitSqQueue(SqQueue &Q)
{
    Q.front = Q.rear = Q.size = 0;
    return true;
}

bool isQuereEmpty(SqQueue Q)
{
    return Q.size == 0;
}
//队列入队
bool enQueue(SqQueue &Q, int e)
{
    if(Q.size == MaxSize)
    {
        cout<<"队列已满，入队失败"<<endl;
        return false;
    }
    Q.data[Q.rear] = e;
    Q.rear = (Q.rear + 1) % MaxSize;
    Q.size++;
    return true;
}

//队列出队
bool deQueue(SqQueue &Q, int &e)
{
    if(Q.size == 0)
    {
        cout<<"队列为空，出队失败"<<endl;
        return false;
    }
    e = Q.data[Q.front];
    Q.front = (Q.front + 1) % MaxSize;
    Q.size--;
    return true;
}

bool GetElem(SqQueue &Q, int &e)
{
    if(Q.size == 0)
    {
        cout<<"队列为空，无法获取队头元素"<<endl;
        return false;
    }
    e = Q.data[Q.front];
    return true;
}

int main()
{
    SqQueue Q;
    InitSqQueue(Q);
    
    // 测试入队
    enQueue(Q, 10);
    enQueue(Q, 20);
    enQueue(Q, 30);
    
    // 测试获取队头
    int e;
    GetElem(Q, e);
    cout << "队头元素：" << e << endl;  // 输出：10
    
    // 测试出队
    deQueue(Q, e);
    cout << "出队：" << e << endl;  // 输出：10
    
    // 测试队空判断
    cout << "队列是否为空：" << (isQuereEmpty(Q) ? "是" : "否") << endl;
    
    return 0;
}