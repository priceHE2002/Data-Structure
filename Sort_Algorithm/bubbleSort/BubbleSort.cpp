#include<iostream>
#include <vector>
using namespace std;

void bubbleSort(vector<int> &array,int n)
{
    int i,j;
    for(i=0;i<n-1;i++)
    {
        for(j=0;j<n-i-1;j++)
        {
            if(array[j]>array[j+1])
            {
                int temp = array[j];
                array[j]=array[j+1];
                array[j+1]=temp;
            }
        }
    }
}

int main()
{
    vector<int> myArray = {1,3,4,2,5,6};
    int N=myArray.size();
    bubbleSort(myArray,N);
    cout<<"Sorted array: ";
    for (int i=0;i<N;i++)
    {
        cout<<myArray[i]<<" ";
    }
    cout<<endl;
    return 0;
}