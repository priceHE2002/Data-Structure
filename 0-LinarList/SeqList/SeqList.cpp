// SeqList.cpp
#include "SeqList.h"
#include <iostream>
using namespace std;

void InitSeqList(SeqList &List)
{
    List.data = new int[InitSize];
    List.length = 0;
    List.MaxSize = InitSize;
}

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

void PrintSeqList(SeqList List)
{
    cout << "该数组为" << endl;
    
    for(int i = 0; i < List.length; i++)
    {
        cout << List.data[i] << " ";
    }
    
    cout << endl;
}