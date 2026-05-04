#include<iostream>
#include <vector>

void selectSort(std::vector<int> &array,int n)
{
    for(int i=0;i<n-1;i++)
    {
        int minIndex=i;
        for(int j=i+1;j<n-1;j++)
        {
            if(array[j]<array[minIndex])
            {
                minIndex=j;
            }
        }
        if(minIndex!=i)
            {
                int temp = array[i];
                array[i]=array[minIndex];
                array[minIndex]=temp;
            }
    }
}
int main()
{
    std::vector<int> myArray= {1,3,4,2,5,6};
    int N=myArray.size();
    selectSort( myArray,N);
    std::cout<<"Sorted array: ";
    for (int i=0;i<N;i++)
    {
        std::cout<<myArray[i]<<" ";
    }
    std::cout<<std::endl;
    return 0;
}