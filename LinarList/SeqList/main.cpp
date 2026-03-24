// main.cpp
#include "SeqList.h"
#include <iostream>
using namespace std;

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