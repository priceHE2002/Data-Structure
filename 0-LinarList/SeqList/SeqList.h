#ifndef SEQLIST_H
#define SEQLIST_H

#define InitSize 10
typedef struct{
    int* data;
    int length;
    int MaxSize;
}SeqList;

// 函数声明
void InitSeqList(SeqList &List);
void IncreaseSeqList(SeqList &List, int num);
void SeqlistInsert(SeqList &List, int i, int e);
void SeqlistDelete(SeqList &List, int i);
void LocateElem(SeqList List, int e);
void PrintSeqList(SeqList List);

#endif