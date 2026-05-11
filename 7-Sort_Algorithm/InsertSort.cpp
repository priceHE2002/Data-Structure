#include<iostream>
using namespace std;

//直接插入排序算法
void DirectInsertSort(int array[], int n)
{
    int i, j ,temp;
    for (int i = 1; i < n ; i++)
    {
        if(array[i] < array[i-1])
        {
            temp = array[i];
            for(j = i - 1; j >= 0 && array[j] > temp ;j--)
            {
                array[j+1] = array[j];
            }
            array[j+1] = temp;
        }
    }
}

//折半插入排序算法
void BinaryInsertSort(int array[] , int n)
{
    int i, j,temp, low, mid, high;
    for (i = 1; i < n ; i++)
    {
        temp = array[i];
        low = 0;
        high = i - 1;
        while(low <= high)//折半查找
        {
            mid = (low + high)/2;
            if(array[mid] > temp)
            {
                high = mid - 1;//查找左子表
            }
            else{
                low = mid + 1;//查找右子表
            }
        }
        for (j = i - 1; j > high; j--)
        {
            array[j + 1] = array[j];
        }
        array[low] = temp;
    }
}

void PrintArray(int array[], int n)
{
    for (int i = 0; i < n; i++)
        cout << array[i] << " ";
    cout << endl;
}

void CopyArray(int src[], int dst[], int n)
{
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

int main()
{
    int arr1[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int arr2[n1];

    cout << "========== 测试1: 随机数组 ==========" << endl;
    cout << "原始数组:           ";
    PrintArray(arr1, n1);

    CopyArray(arr1, arr2, n1);
    DirectInsertSort(arr1, n1);
    cout << "直接插入排序结果:   ";
    PrintArray(arr1, n1);

    BinaryInsertSort(arr2, n1);
    cout << "折半插入排序结果:   ";
    PrintArray(arr2, n1);
    cout << endl;

    int arr3[] = {1, 2, 3, 4, 5, 6};
    int n2 = sizeof(arr3) / sizeof(arr3[0]);
    int arr4[n2];

    cout << "========== 测试2: 已排序数组 ==========" << endl;
    cout << "原始数组:           ";
    PrintArray(arr3, n2);

    CopyArray(arr3, arr4, n2);
    DirectInsertSort(arr3, n2);
    cout << "直接插入排序结果:   ";
    PrintArray(arr3, n2);

    BinaryInsertSort(arr4, n2);
    cout << "折半插入排序结果:   ";
    PrintArray(arr4, n2);
    cout << endl;

    int arr5[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int n3 = sizeof(arr5) / sizeof(arr5[0]);
    int arr6[n3];

    cout << "========== 测试3: 逆序数组 ==========" << endl;
    cout << "原始数组:           ";
    PrintArray(arr5, n3);

    CopyArray(arr5, arr6, n3);
    DirectInsertSort(arr5, n3);
    cout << "直接插入排序结果:   ";
    PrintArray(arr5, n3);

    BinaryInsertSort(arr6, n3);
    cout << "折半插入排序结果:   ";
    PrintArray(arr6, n3);
    cout << endl;

    int arr7[] = {4, 4, 2, 2, 3, 3, 1, 1};
    int n4 = sizeof(arr7) / sizeof(arr7[0]);
    int arr8[n4];

    cout << "========== 测试4: 含重复元素 ==========" << endl;
    cout << "原始数组:           ";
    PrintArray(arr7, n4);

    CopyArray(arr7, arr8, n4);
    DirectInsertSort(arr7, n4);
    cout << "直接插入排序结果:   ";
    PrintArray(arr7, n4);

    BinaryInsertSort(arr8, n4);
    cout << "折半插入排序结果:   ";
    PrintArray(arr8, n4);
    cout << endl;

    int arr9[] = {42};
    int n5 = sizeof(arr9) / sizeof(arr9[0]);
    int arr10[n5];

    cout << "========== 测试5: 单元素数组 ==========" << endl;
    cout << "原始数组:           ";
    PrintArray(arr9, n5);

    CopyArray(arr9, arr10, n5);
    DirectInsertSort(arr9, n5);
    cout << "直接插入排序结果:   ";
    PrintArray(arr9, n5);

    BinaryInsertSort(arr10, n5);
    cout << "折半插入排序结果:   ";
    PrintArray(arr10, n5);

    return 0;
}