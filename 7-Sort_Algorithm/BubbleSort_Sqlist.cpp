#include<iostream>
#include <vector>
using namespace std;

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void bubbleSort(int array[] ,int n)
{
    for(int i = 0 ; i < n ; i++)
    {
        bool flag = false;
        for(int j = n - 1; j > i ; j--)
        {
            if(array[j - 1] > array[j])
            {
                swap(array[j - 1],array[j]);
                flag = true;
            }
        }
        if(flag == false)
        {
            return;
        }
    }
}


int main()
{
    vector<int> myArray = {1,3,4,2,5,6};
    int N=myArray.size();
    bubbleSort(myArray.data(), N);
    cout<<"Sorted array: ";
    for (int i=0;i<N;i++)
    {
        cout<<myArray[i]<<" ";
    }
    cout<<endl;
    return 0;
}