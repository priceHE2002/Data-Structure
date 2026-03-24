#include <iostream>
using namespace std;

struct LinkNode
{
    int data;
    LinkNode *next;
};

struct LinkQueue
{
    LinkNode *front, *rear;
};

bool InitLinkQueue(LinkQueue &Q)
{
    LinkNode *node = new LinkNode;
    if(node == nullptr)
    {
        return false;
    }
    node->next = nullptr;
    Q.front = Q.rear = node;
    return true;
}

bool IsLinkQueueEmpty(LinkQueue Q)
{
    return Q.front == Q.rear;
}
//入队
bool enLinkQueue(LinkQueue &Q, int e)
{
    LinkNode *newNode = new LinkNode;
    if(newNode == nullptr)
    {
        cout<<"新结点内存分配失败，入队失败"<<endl;
        return false;
    }
    newNode->data =e;
    newNode->next = nullptr;
    Q.rear->next = newNode;
    Q.rear = Q.rear->next;
    return true;
}

//出队
bool deLinkQueue(LinkQueue &Q, int &e)
{
    if(Q.front == Q.rear)
    {
        cout<<"空列为空，出队失败"<<endl;
        return false;
    }
    LinkNode *p = Q.front->next;
    e = p->data;
    Q.front->next = p->next;
    if(Q.rear == p)
    {
        Q.rear = Q.front;
    }
    delete p;
    return true;
}

//队长
int length(LinkQueue Q)
{
    int count = 0;
    LinkNode *p = Q.front;
    while(p != Q.rear)
    {
        p = p->next;
        count++;
    }
    return count;
}

bool DestroyLinkQueue(LinkQueue &Q)
{
    LinkNode *p = Q.front;  //从队头开始
    while(p != nullptr)     //遍历所有结点
    {
        Q.front = p->next;  //先保存下一个结点
        delete p;           //删除当前结点
        p = Q.front;        //移动到下一个结点
    }
    Q.front = Q.rear = nullptr;  //清空队列指针
    return true;
}

int main()
{
    LinkQueue Q;
    InitLinkQueue(Q);
    
    // 测试入队
    enLinkQueue(Q, 10);
    enLinkQueue(Q, 20);
    enLinkQueue(Q, 30);
    
    cout << "队列长度：" << length(Q) << endl;  // 输出：3
    
    // 测试出队
    int e;
    deLinkQueue(Q, e);
    cout << "出队：" << e << endl;  // 输出：10
    
    cout << "队列长度：" << length(Q) << endl;  // 输出：2
    
    // 测试销毁
    DestroyLinkQueue(Q);
    cout << "队列已销毁" << endl;
    
    return 0;
}