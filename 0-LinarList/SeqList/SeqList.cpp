#include <iostream>
using namespace std;

#define InitSize 10

typedef struct{
    int* data;
    int length;
    int MaxSize;
}SeqList;

// 初始化顺序表
void InitSeqList(SeqList &List)
{
    List.data = new int[InitSize];
    List.length = 0;
    List.MaxSize = InitSize;
}

// 扩容
void IncreaseSeqList(SeqList &List, int num)
{
    int* p = List.data;
    int* newData = new int[List.MaxSize + num];
    
    if(newData == nullptr)
    {
        cout << "内存分配失败" << endl;
        return;
    }
    
    for(int i = 0; i < List.length; i++)
    {
        newData[i] = p[i];
    }
    
    List.data = newData;
    List.MaxSize = List.MaxSize + num;
    delete[] p;
}

// 插入
void SeqlistInsert(SeqList &List, int i, int e)
{
    if(i < 1 || i > List.length + 1)
    {
        cout << "插入失败 i的输入非法" << endl;
        return;
    }
    
    if(List.length >= List.MaxSize)
    {
        cout << "插入失败 SeqList的长度达到上限" << endl;
        return;
    }
    
    for(int j = List.length; j > i - 1; j--)
    {
        List.data[j] = List.data[j - 1];
    }
    
    List.data[i - 1] = e;
    List.length++;
}

// 删除
void SeqlistDelete(SeqList &List, int i)
{
    if(i < 1 || i > List.length)
    {
        cout << "删除非法" << endl;
        return;
    }
    
    for(int j = i - 1; j < List.length - 1; j++)
    {
        List.data[j] = List.data[j + 1];
    }
    
    List.length--;
}

// 查找
void LocateElem(SeqList List, int e)
{
    int isElemExist = 0;
    
    for(int i = 0; i < List.length; i++)
    {
        if(List.data[i] == e)
        {
            cout << "值为" << e << "的元素在数组中的位序为" << i + 1 << endl;
            isElemExist = 1;
        }
    }
    
    if(isElemExist == 0)
    {
        cout << "该数组不存在值为" << e << "的元素" << endl;
    }
}

// 打印
void PrintSeqList(SeqList List)
{
    cout << "该数组为" << endl;
    
    for(int i = 0; i < List.length; i++)
    {
        cout << List.data[i] << " ";
    }
    
    cout << endl;
}

int main()
{
    SeqList L;
    
    InitSeqList(L);
    cout << "初始化成功，容量: " << L.MaxSize << endl;
    
    IncreaseSeqList(L, 5);
    cout << "扩容成功，新容量: " << L.MaxSize << endl;
    
    SeqlistInsert(L, 1, 1);
    SeqlistInsert(L, 2, 3);
    SeqlistInsert(L, 3, 5);
    PrintSeqList(L);
    
    SeqlistDelete(L, 1);
    cout << "删除第1个元素后：" << endl;
    PrintSeqList(L);
    
    LocateElem(L, 3);
    LocateElem(L, 6);
    
    delete[] L.data;
    
    return 0;
}
