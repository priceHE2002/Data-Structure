#include <cstddef>
#include <iostream>
#include <utility>
using namespace std;

typedef struct LNode
{
    int data;
    LNode *next;
}LNode, *LinkList;

bool InitLinkListWithHead(LinkList &L)//初始化带头结点的单链表
{
    L = new LNode;
    if(L == nullptr)
    {
        return false;
    }
    L->next = nullptr;
    return true;
}

//按位查找，返回第i个元素（带头结点）
LNode *GetElem(LinkList L, int i)
{
    if(i<0)
    {
        return nullptr;
    }
    LNode *p = L;
    int j = 0;
    while(p != nullptr && j<i)
    {
        p = p->next;
        j++;
    }
    return p;
}

//按值查找，找到第一个数据域为e的结点，并返回这个结点
LNode *LocateElem(LinkList L, int e)
{
    LNode *p =L->next;
    while(p->data != e && p != nullptr)
    {
        p = p->next;
    }
    return p;
}

//求单链表的长度
int length(LinkList L)
{
    int len = 0;
    LNode *p = L;
    while(p->next != nullptr)
    {
        p = p->next;
        len++;
    }
    return len;
}

//在p结点后插入一个元素
bool InsertNextNode(LNode *p, int e)
{
    if(p == nullptr)
    {
        return false;
    }
    LNode *newNode = new LNode;
    if(newNode == nullptr)
    {
        cout<<"新结点内存分配失败"<<endl;
        return false;
    }
    newNode->data = e;
    newNode->next = p->next;
    p->next = newNode;
    return true;
}

bool ListInsert(LinkList &L,int i,int e)//在位序i处插入元素e
{
    if(i < 1)
    {
        return false;
    }
    LNode *p = GetElem(L,i-1);//找到第i-1个结点
    if (p == nullptr)
    {
        return false;
    }
    return InsertNextNode(p, e);
}

bool InsertPriorNode(LinkList &L, LNode *p, LNode *q)//在p结点前插入q结点
{
    if(p == nullptr || q == nullptr)
    {
        return false;
    }

    // 如果 p 是头结点，不能使用交换数据的方法
    if(p == L)
    {
        return false;  // 或者使用其他方法
    }
    
    q->next = p->next;
    p->next = q;
    int temp = p->data;
    p->data = q->data;
    q->data = temp;
    return true;
}

bool ListDelete(LinkList &L, int i, int &e)//删除位序为i的结点，并返回该结点的值
{
    if(i < 1)
    {
        return false;
    }
    LNode *p = GetElem(L, i-1);//找到第i-1个结点
    if(p == nullptr)
    {
        return false;
    }

    if(p->next == nullptr)
    {
        return false;
    }
    LNode *deleteNode = p->next;
    e = deleteNode->data;
    p->next = deleteNode->next;
    delete deleteNode;
    return true;
}

bool DeleteNode(LNode *p)//删除某个结点（偷梁换柱法）
{
    if(p == nullptr)
    {
        return false;
    }
    //如果p是最后一个结点，无法使用这种方法
    if(p->next == nullptr)
    {
        return false;
    }
    LNode *deleteNode = p->next;
    p->data = deleteNode->data;
    p->next = deleteNode->next;
    delete deleteNode;

    return true;
}

bool DestroyList(LinkList &L)//释放链表
{
    if(L == nullptr)
    {
        return false;
    }
    LNode *current = L;
    LNode *temp;
    while(current != nullptr)
    {
        temp = current;
        current = current->next;
        delete temp;
    }
    L = nullptr;
    return true;
}

bool PrintLinkList(LinkList L)
{
    if(L == nullptr)
    {
        return false;
    }

    LNode *p = L->next;
    while(p != nullptr)
    {
        cout<<p->data<<" ";
        p = p->next;
    }
    cout<<endl;
    return true;
}

LinkList List_TailInsert_WithFunc(LinkList &L)
{
    int x;
    InitLinkListWithHead(L);
    LNode *tailNode = L;
    cout<<"调用封装好的函数的尾插法，请输入链表各结点的值(输入666结束):"<<endl;
    cin >> x;
    while(x != 666)
    {
        if(!InsertNextNode(tailNode, x))
        {
            cout<<"插入结点失败"<<endl;
            DestroyList(L);
            return nullptr;
        }
        tailNode = tailNode->next;
        cin >> x;
    }
    return L;
}

LinkList List_TailInsert_WithOutFunc(LinkList &L)
{
    int x;
    L = new LNode;
    if(L == nullptr)
    {
        return nullptr;
    }
    L->next = nullptr;
    LNode *tailNode = L;
    cout<<"不调用封装好的函数的尾插法，请输入链表各结点的值(输入666结束):"<<endl;
    cin >> x;
    while(x != 666)
    {
        LNode *newNode = new LNode;
        if(newNode == nullptr)
        {
            cout<<"插入失败"<<endl;
            DestroyList(L);
            return nullptr;
        }
        newNode->data = x;
        tailNode->next = newNode;
        tailNode = tailNode->next;
        cin >> x;
    }
    return L;
}

LinkList List_HeadInsert_WithFunc(LinkList &L)
{
    int x;
    InitLinkListWithHead(L);
    LNode *headNode = L;
    cout<<"调用封装好的函数的头插法，请输入链表各结点的值(输入666结束):"<<endl;
    cin >> x;
    while(x != 666)
    {
        if(!InsertNextNode(headNode, x))
        {
            cout<<"插入结点失败"<<endl;
            DestroyList(L);
            return nullptr;
        }
        cin >> x;
    }
    return L;
}


LinkList List_HeadInsert_WithOutFunc(LinkList &L)
{
    int x;
    //初始化
    L = new LNode;
    if (L == nullptr)
    {
        return nullptr;
    }
    L->next = nullptr;
    //
    LNode* headNode = L;
    cout<<"不调用封装好的函数的头插法，请输入链表各结点的值(输入666结束):"<<endl;
    cin >> x;
    while(x != 666)
    {
        LNode *newNode =new LNode;
        if(newNode == nullptr)
        {
            cout<<"插入失败"<<endl;
            DestroyList(L);
            return nullptr;
        }
        newNode->data = x;
        newNode->next = headNode->next;
        headNode->next = newNode;
        cin >> x;
    }
    return L;
}

bool ReverseLink(LinkList &L)
 {
    LNode *p = L->next;
    L->next = nullptr;
    LNode *q;
    while( p!= nullptr)
    {
        q = p->next;
        p->next = L->next;
        L->next = p;
        p = q;
    }
    return true;
 }

//交换p结点和p->next的data域
void swapDataWithNextNode(LNode *p)
{
    int temp = p->data;
    p->data = p->next->data;
    p->next->data = temp;
}

void bubbleSort(LinkList &L)
{
    LNode *p;
    int n = length(L);//n为链表长度
    for(int i = 0; i < n; i++)
    {
        p = L->next;
        bool flag = false;
        for(int j = 0; j < n - i - 1; j++)
        {
            if(p->data > p->next->data)
            {
                swapDataWithNextNode(p);
                flag = true;       
            }
            p = p->next;
        }
        if(flag == false)
        {
            return;
        }
    }
}

void BuildListFromArray(LinkList &L, int arr[], int n)
{
    InitLinkListWithHead(L);
    LNode *tail = L;
    for (int i = 0; i < n; i++)
    {
        LNode *newNode = new LNode;
        newNode->data = arr[i];
        newNode->next = nullptr;
        tail->next = newNode;
        tail = newNode;
    }
}

int main()
{
    cout << "========== 测试1: 随机链表 ==========" << endl;
    int arr1[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    LinkList L1;
    BuildListFromArray(L1, arr1, n1);
    cout << "排序前: ";
    PrintLinkList(L1);
    bubbleSort(L1);
    cout << "排序后: ";
    PrintLinkList(L1);
    DestroyList(L1);
    cout << endl;

    cout << "========== 测试2: 已排序链表 ==========" << endl;
    int arr2[] = {1, 2, 3, 4, 5, 6};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    LinkList L2;
    BuildListFromArray(L2, arr2, n2);
    cout << "排序前: ";
    PrintLinkList(L2);
    bubbleSort(L2);
    cout << "排序后: ";
    PrintLinkList(L2);
    DestroyList(L2);
    cout << endl;

    cout << "========== 测试3: 逆序链表 ==========" << endl;
    int arr3[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    LinkList L3;
    BuildListFromArray(L3, arr3, n3);
    cout << "排序前: ";
    PrintLinkList(L3);
    bubbleSort(L3);
    cout << "排序后: ";
    PrintLinkList(L3);
    DestroyList(L3);
    cout << endl;

    cout << "========== 测试4: 含重复元素 ==========" << endl;
    int arr4[] = {4, 4, 2, 2, 3, 3, 1, 1};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    LinkList L4;
    BuildListFromArray(L4, arr4, n4);
    cout << "排序前: ";
    PrintLinkList(L4);
    bubbleSort(L4);
    cout << "排序后: ";
    PrintLinkList(L4);
    DestroyList(L4);
    cout << endl;

    cout << "========== 测试5: 单节点链表 ==========" << endl;
    int arr5[] = {42};
    int n5 = sizeof(arr5) / sizeof(arr5[0]);
    LinkList L5;
    BuildListFromArray(L5, arr5, n5);
    cout << "排序前: ";
    PrintLinkList(L5);
    bubbleSort(L5);
    cout << "排序后: ";
    PrintLinkList(L5);
    DestroyList(L5);
    cout << endl;

    cout << "========== 测试6: 空链表 ==========" << endl;
    LinkList L6;
    InitLinkListWithHead(L6);
    cout << "排序前: (空)" << endl;
    bubbleSort(L6);
    cout << "排序后: (空)" << endl;
    DestroyList(L6);

    return 0;
}