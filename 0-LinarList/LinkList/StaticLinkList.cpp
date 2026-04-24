#include <iostream>
using namespace std;

#define MaxSize 10
struct node
{
    int data;
    int next; //下一个元素的数组下标
};

void InitStaticLinkList(node (&a)[MaxSize])
{
    for(int i = 1;i < MaxSize; i++)
    {
        a[i].data = -100;
        a[i].next = -2;
    }
    a[0].data = -100;
    a[0].next = -1;
}

int length(node a[])
{
    int len = 0;
    int p = a[0].next;
    while(p != -1)
    {
        len++;
        p = a[p].next;
    }
    return len;
}
//找到位序为i的结点下标p(即第i-1个结点的next)
bool LocateElem(node a[], int i, int &p)
{
    if(i < 1 || i>length(a))
    {
        cout<<"不存在位序为"<<i<<"的元素"<<endl;
        return false;
    }
    p = a[0].next;
    int count = 0;
    while(count < i-1)
    {
        p = a[p].next;
        count++;
    }
    return true;
}
/*
插入位序为i的结点
1.先找到数组中一个空结点的下标p，并存入数据
2.找到位序为i-1的结点的下标，
3.将新结点的next修改为位序i-1结点的next
3.将位序为i-1的结点的next修改为p
*/
bool InsertNode(node a[], int i, int e)
{
    if(i < 1 || i > length(a) + 1)
    {
        cout<<"插入位置不合法"<<endl;
        return false;
    }
    
    // 1. 查找空闲空间（使用 for 循环，安全）
    int p1 = -1;
    for(int j = 1; j < MaxSize; j++)
    {
        if(a[j].next == -2)  // -2 表示空闲
        {
            p1 = j;
            break;
        }
    }
    
    // 2. 检查是否有空闲空间
    if(p1 == -1)
    {
        cout<<"空间已满，无法插入"<<endl;
        return false;
    }
    
    // 3. 找到位序为 i-1 的结点
    int p2;
    if(i == 1)  // 在第一个位置插入
    {
        p2 = 0;  // 头结点
    }
    else
    {
        if(!LocateElem(a, i-1, p2))
        {
            return false;
        }
    }
    
    // 4. 插入新结点
    a[p1].data = e;
    a[p1].next = a[p2].next;
    a[p2].next = p1;
    
    return true;
}

int main()
{
    node a[MaxSize];
    InitStaticLinkList(a);
    
    // 测试插入
    InsertNode(a, 1, 10);  // 插入第 1 个元素 10
    InsertNode(a, 2, 20);  // 插入第 2 个元素 20
    InsertNode(a, 2, 15);  // 在第 2 个位置插入 15
    
    // 打印链表
    cout << "链表长度：" << length(a) << endl;
    
    int p = a[0].next;
    while(p != -1)
    {
        cout << a[p].data << " ";
        p = a[p].next;
    }
    cout << endl;
    return 0;
}