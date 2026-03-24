#include<iostream>
#include<vector>
using namespace std;

void insertSort(int array[], int n)
{
    for(int i = 1; i < n; i++)
    {
        int temp = array[i];
        int j = i - 1;
    
        while(j >= 0 && array[j] > temp)
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = temp;
    }
}

int main()
{
    vector<int> myArray;
    int num;
    cout << "Enter numbers (enter -1 to finish):" << endl;
    while(true)
    {
        cout << "Input number: ";
        cin >> num;
        if(num == -1)
        {
            break;
        }
        myArray.push_back(num);
    }
    int N = myArray.size();
    if(N == 0)
    {
        cout << "No numbers entered!" << endl;
        return 0;
    }   
    insertSort(myArray.data(), N);
    cout << "Sorted array: ";
    for(int i = 0; i < N; i++)
    {
        cout << myArray[i] << " ";
    }
    cout << endl;
    return 0;
}