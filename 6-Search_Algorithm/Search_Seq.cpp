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

//顺序查找，在SSTable T中查找第一个值为a的元素，找到则返回a的下标，找不到返回-1
int Search_Seq(SSTable T,int a)
{
    int i;
    for(i = 0; i < T.tableLen && T.data[i] != a; i++);
    return i == T.tableLen ? -1 : i;
}

int main()
{
    SSTable T;
    if (!InitSSTable(T))
    {
        cout << "初始化失败" << endl;
        return -1;
    }

    int arr[] = {3, 7, 1, 9, 5, 2};
    int n = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < n; i++)
        T.data[i] = arr[i];
    T.tableLen = n;

    cout << "查找表中元素: ";
    for (int i = 0; i < T.tableLen; i++)
        cout << T.data[i] << " ";
    cout << endl;

    int target = 9;
    int result = Search_Seq(T, target);
    if (result != -1)
        cout << "元素 " << target << " 的下标为: " << result << endl;
    else
        cout << "未找到元素 " << target << endl;

    target = 8;
    result = Search_Seq(T, target);
    if (result != -1)
        cout << "元素 " << target << " 的下标为: " << result << endl;
    else
        cout << "未找到元素 " << target << endl;

    delete[] T.data;

    return 0;
}