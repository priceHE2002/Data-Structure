#include<iostream>
using namespace std;
#define Maxsize 100
//顺序查找算法
struct SSTable{
    int *data; //动态数组基址
    int tableLen; //查找表长度
};

bool InitSSTable(SSTable &T)
{
    T.data = new int[Maxsize];
    if(T.data == nullptr)
    {
        return false;
    }
    T.tableLen = 0;
    return true;
}

int Binary_Search(SSTable T,int key)
{
    int low = 0 , high = T.tableLen - 1;
    int mid;
    while(low <= high)
    {
        mid = (low + high) / 2;
        if(key == T.data[mid])
        {
            return mid;
        }
        else if(key < T.data[mid])
        {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }
    return -1;
}